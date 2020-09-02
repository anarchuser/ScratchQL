#include "Wrapper.h"

Response Wrapper::unwrap (::RPCServer::Maybe<::RPCServer::Table>::Reader const & reader) {
    if (reader.isEmpty()) return std::nullopt;
    return unwrap (reader.getValue());
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrap (Response response) {
    if (!response) {
        kj::Own <capnp::MallocMessageBuilder> responseBuilder = kj::heap <capnp::MallocMessageBuilder> ();
        responseBuilder->initRoot <RPCServer::Maybe <::RPCServer::Table>> ().setEmpty ();
        return responseBuilder;
    }
    auto const & table = response.value();
    kj::Own <capnp::MallocMessageBuilder> tableBuilder = kj::heap <capnp::MallocMessageBuilder>();
    RPCServer::Table::Builder builder = tableBuilder->initRoot <::RPCServer::Table>();

    auto metaBuilder = builder.initMeta (table->columnCount ());
    std::vector <Meta> meta = table->getMeta();
    std::size_t idx = 0;
    for (auto const & col : meta) {
        metaBuilder.setWithCaveats (idx++, wrap (col)->getRoot <RPCServer::Table::Meta>().asReader());
    }

    auto contentBuilder = builder.initContent (table->rowCount ());
    for (std::size_t r = 0; r < table->rowCount (); r++) {
        auto rowBuilder = contentBuilder [r].initData (table->columnCount ());
        std::size_t c = 0;
        for (auto const & cell : table->readRowAsVector (r)) {
            auto cellBuilder = wrap (cell);
            rowBuilder.setWithCaveats (c++, cellBuilder->getRoot <RPCServer::Table::Cell>());
            LOG_ASSERT (cell == unwrap (rowBuilder.asReader()[c-1]));
        }
    }

    return tableBuilder;
}

kj::Own <Table> Wrapper::unwrap (::RPCServer::Table::Reader const & reader) {
    std::vector <Meta> metae;
    for (auto const & meta : reader.getMeta ()) metae.push_back (unwrap (meta));
    kj::Own <Table> table = kj::heap <Table> (metae, std::string(""), std::string(""));
    //TODO: Serialise Database- and Tablename

    for (auto const & row : reader.getContent ()) {
        std::vector <Cell> newRow;
        for (auto const & wrappedCell : row.getData()) newRow.push_back (unwrap (wrappedCell));
        table->createRow (newRow);
    }
    return table;
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrap (Meta const & meta) {
    kj::Own <capnp::MallocMessageBuilder> metaBuilder = kj::heap <capnp::MallocMessageBuilder>();
    RPCServer::Table::Meta::Builder builder = metaBuilder->initRoot <RPCServer::Table::Meta>();
    builder.setName (meta.name);
    builder.setDataType (meta.dataType);
    builder.setKeyType (meta.keyType);
    builder.setIndex (meta.index);
    builder.setNullable (meta.nullable);

    auto dataBuilder = builder.initReference();
    meta.reference.index() ?
        dataBuilder.setValue(std::get <std::string> (meta.reference)) :
        dataBuilder.setEmpty();

    return metaBuilder;
}

Meta Wrapper::unwrap (RPCServer::Table::Meta::Reader const & reader) {
    Meta meta;
    meta.name     = reader.getName ();
    meta.dataType = (CellType) reader.getDataType();
    meta.keyType  = (KeyType)  reader.getKeyType ();
    meta.index    = reader.getIndex();
    meta.nullable = reader.getNullable();

    auto ref = reader.getReference ();
    if (ref.hasValue()) meta.reference = Key::Reference (ref.getValue());

    return std::move (meta);
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrap (Cell const & cell) {
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

Cell Wrapper::unwrap (RPCServer::Table::Cell::Reader const & cell) {
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