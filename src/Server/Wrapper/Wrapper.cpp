#include <capnp/message.h>
#include "Wrapper.h"

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrapResponse (Response response) {
    switch (response.index()) {
        case ResponseType::VOID: {
            kj::Own <capnp::MallocMessageBuilder> responseBuilder = kj::heap <capnp::MallocMessageBuilder> ();
            responseBuilder->initRoot <RPCServer::Response> ().initData ().setVoid ();
            return responseBuilder; }
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

    auto metaBuilder = builder.initMeta (table->columnCount ());
    std::vector <Meta> meta = table->getMeta();
    std::size_t idx = 0;
    for (auto const & col : meta) {
        metaBuilder.setWithCaveats (idx++, wrapMeta (col)->getRoot <RPCServer::Table::Meta>().asReader());
    }

    auto contentBuilder = builder.initContent (table->rowCount ());
    for (std::size_t r = 0; r < table->rowCount (); r++) {
        auto rowBuilder = contentBuilder [r].initData (table->columnCount ());
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
    std::vector <Meta> metae;
    for (auto const & meta : reader.getMeta ()) metae.push_back (unwrapMeta (meta));
    kj::Own <Table> table = kj::heap <Table> (metae, std::string(""), std::string(""));
    //TODO: Serialise Database- and Tablename

    for (auto const & row : reader.getContent ()) {
        std::vector <Cell> newRow;
        for (auto const & wrappedCell : row.getData()) newRow.push_back (unwrapCell (wrappedCell));
        table->createRow (newRow);
    }
    return table;
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrapMeta (Meta const & meta) {
    kj::Own <capnp::MallocMessageBuilder> metaBuilder = kj::heap <capnp::MallocMessageBuilder>();
    RPCServer::Table::Meta::Builder builder = metaBuilder->initRoot <RPCServer::Table::Meta>();
    builder.setName (meta.name);
    builder.setDataType (meta.dataType);
    builder.setKeyType (meta.keyType);
    builder.setIndex (meta.index);
    builder.setNullable (meta.nullable);

    auto dataBuilder = builder.initReference().initData();
    meta.reference.index() ?
        dataBuilder.setTable (std::get <std::string> (meta.reference)) :
        dataBuilder.setUnary();

    return metaBuilder;
}

Meta Wrapper::unwrapMeta (RPCServer::Table::Meta::Reader const & reader) {
    Meta meta;
    meta.name     = reader.getName ();
    meta.dataType = (CellType) reader.getDataType();
    meta.keyType  = (KeyType)  reader.getKeyType ();
    meta.index    = reader.getIndex();
    meta.nullable = reader.getNullable();

    auto ref = reader.getReference ().getData();
    if (ref.isTable()) meta.reference = Key::Reference (ref.getTable());

    return std::move (meta);
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrapCell (Cell const & cell) {
    kj::Own <capnp::MallocMessageBuilder> cellBuilder = kj::heap <capnp::MallocMessageBuilder> ();
    RPCServer::Table::Cell::Builder builder = cellBuilder->initRoot <::RPCServer::Table::Cell>();

    switch (cell.index ()) {
        case UNARY:
            LOG (INFO) << "Set Cell to Unary";
            builder.setUnary();
            break;
        case BINARY:
            LOG (INFO) << "Set Cell to Binary: " << std::get <bool> (cell);
            builder.setBinary (std::get <bool> (cell));
            break;
        case SHORT:
            LOG (INFO) << "Set Cell to Short: " << std::get <short> (cell);
            builder.setShort (std::get <short> (cell));
            break;
        case LONG:
            LOG (INFO) << "Set Cell to Long: " << std::get <long> (cell);
            builder.setLong (std::get <long> (cell));
            break;
        case TEXT:
            LOG (INFO) << "Set Cell to Text: " << std::get <std::string> (cell);
            builder.setText (std::get <std::string> (cell));
            break;
        default:
            LOG (FATAL) << "Cell Wrapper went insane: " << cell.index() << " not in range [0, 5)";
    }
    LOG_ASSERT (cell.index() == builder.asReader().which());
    LOG_ASSERT (cell.index() == cellBuilder->getRoot <RPCServer::Table::Cell>().which());

    return cellBuilder;
}

Cell Wrapper::unwrapCell (RPCServer::Table::Cell::Reader const & cell) {
    switch (cell.which()) {
        case RPCServer::Table::Cell::UNARY:
            return Cell();
        case RPCServer::Table::Cell::BINARY:
            return Cell (cell.getBinary());
        case RPCServer::Table::Cell::SHORT:
            return Cell ((short) cell.getShort());
        case RPCServer::Table::Cell::LONG:
            return Cell ((long) cell.getLong());
        case RPCServer::Table::Cell::TEXT:
            return Cell (cell.getText());
        default:
            LOG (FATAL) << "Cell Unwrapper went insane: " << (short) cell.which() << " not in range [0, 5)";
    }
};

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */