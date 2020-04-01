#include <capnp/message.h>
#include "Wrapper.h"

kj::Own <capnp::MallocMessageBuilder> const & Wrapper::wrapResponse (Response response) {
    switch (response.index()) {
        case ResponseType::VOID:
            static kj::Own <capnp::MallocMessageBuilder> responseBuilder = kj::heap <capnp::MallocMessageBuilder>();
            responseBuilder->initRoot <RPCServer::Response>().initData().setVoid();
            return responseBuilder;
        case ResponseType::TABLE:
            return std::move (wrapTable (std::get <kj::Own <Table const>> (response)));
        default:
            LOG (FATAL) << "Insane Response index received: got {" << response.index() <<
            "}, expected VOID {0} or TABLE {1}";
    }
}

Response Wrapper::unwrapResponse (::RPCServer::Response::Reader const & reader) {
    auto data = reader.getData();
    switch (data.which()) {
        case RPCServer::Response::Data::VOID:
            return std::monostate();
        case RPCServer::Response::Data::TABLE:
            return unwrapTable (data.getTable());
        default:
            LOG (FATAL) << "Response Unwrapper went insane: " << (short) data.which() << " not in range [0, 2)";
    }
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrapTable (kj::Own <Table const> const & table) {
    kj::Own <capnp::MallocMessageBuilder> tableBuilder = kj::heap <capnp::MallocMessageBuilder>();
    RPCServer::Table::Builder builder = tableBuilder->initRoot <::RPCServer::Table>();

    auto headerBuilder = builder.initHeader (table->getColumnCount());
    duplicateList (headerBuilder, table->getHeader());

    auto contentBuilder = builder.initContent (table->getRowCount());
    for (std::size_t r = 0; r < table->getRowCount (); r++) {
        auto rowBuilder = contentBuilder [r].initData (table->getColumnCount());
        std::size_t c = 0;
        for (auto const & cell : table->readRowAsVector (r)) {
            auto cellBuilder = wrapCell (cell);
            rowBuilder.setWithCaveats (c++, cellBuilder->getRoot <RPCServer::Table::Cell>());
            LOG_ASSERT (cell == unwrapCell (rowBuilder.asReader()[c-1]));
        }
    }
    return tableBuilder;
}

kj::Own <Table> Wrapper::unwrapTable (::RPCServer::Table::Reader const & reader) {
    std::vector <std::string> header;
    std::vector <KeyTypes> meta;
    for (auto const & column : reader.getHeader()) header.emplace_back (column);
    for (auto const & info : reader.getMeta ()) meta.push_back (KeyTypes (info));
    kj::Own <Table> table = kj::heap <Table> (header);

    for (auto const & row : reader.getContent ()) {
        std::vector <Cell> newRow;
        for (auto const & wrappedCell : row.getData()) newRow.push_back (unwrapCell (wrappedCell));
        table->createRow (newRow);
    }
    return table;
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrapCell (Cell const & cell) {
    kj::Own <capnp::MallocMessageBuilder> cellBuilder = kj::heap <capnp::MallocMessageBuilder> ();
    RPCServer::Table::Cell::Builder builder = cellBuilder->initRoot <::RPCServer::Table::Cell>();
    RPCServer::Table::Cell::Data::Builder builderData = builder.initData();

    switch (cell.index ()) {
        case UNARY:
            LOG (INFO) << "Set Cell to Unary";
            builderData.setUnary();
            break;
        case BINARY:
            LOG (INFO) << "Set Cell to Binary: " << std::get <bool> (cell);
            builderData.setBinary (std::get <bool> (cell));
            break;
        case SHORT:
            LOG (INFO) << "Set Cell to Short: " << std::get <short> (cell);
            builderData.setShort (std::get <short> (cell));
            break;
        case LONG:
            LOG (INFO) << "Set Cell to Long: " << std::get <long> (cell);
            builderData.setLong (std::get <long> (cell));
            break;
        case TEXT:
            LOG (INFO) << "Set Cell to Text: " << std::get <std::string> (cell);
            builderData.setText (std::get <std::string> (cell));
            break;
        default:
            LOG (FATAL) << "Cell Wrapper went insane: " << cell.index() << " not in range [0, 5)";
    }
    LOG_ASSERT (cell.index() == builderData.asReader().which());
    LOG_ASSERT (cell.index() == cellBuilder->getRoot <RPCServer::Table::Cell>().getData().which());

    return cellBuilder;
}

Cell Wrapper::unwrapCell (RPCServer::Table::Cell::Reader const & cell) {
    RPCServer::Table::Cell::Data::Reader data = cell.getData();

    switch (data.which()) {
        case RPCServer::Table::Cell::Data::UNARY:
            return Cell();
        case RPCServer::Table::Cell::Data::BINARY:
            return Cell (data.getBinary());
        case RPCServer::Table::Cell::Data::SHORT:
            return Cell ((short) data.getShort());
        case RPCServer::Table::Cell::Data::LONG:
            return Cell ((long) data.getLong());
        case RPCServer::Table::Cell::Data::TEXT:
            return Cell (data.getText());
        default:
            LOG (FATAL) << "Cell Unwrapper went insane: " << (short) data.which() << " not in range [0, 5)";
    }
};

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */