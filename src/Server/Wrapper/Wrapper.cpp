#include <capnp/message.h>
#include "Wrapper.h"

::RPCServer::Table::Reader Wrapper::wrapTable (Table const & table) {
    ::capnp::MallocMessageBuilder builderBuilder;
    ::RPCServer::Table::Builder builder = builderBuilder.initRoot <::RPCServer::Table>();

    auto headerBuilder = builder.initHeader (table.getColumnCount());
    duplicateList (headerBuilder, table.getHeader());

    auto metaBuilder = builder.initMeta (table.getColumnCount());
    duplicateList (metaBuilder, table.getMeta());

    auto contentBuilder = builder.initContent (table.getRowCount());
    for (std::size_t r = 0; r < table.getRowCount (); r++) {
        auto rowBuilder = contentBuilder [r].initData (table.getColumnCount());
        std::size_t c = 0;
        for (auto const & cell : table.readRowAsVector (r)) rowBuilder.setWithCaveats (c++, wrapCell (cell));
        contentBuilder [r].setData (rowBuilder.asReader());
    }
    return builder.asReader();
}

Table Wrapper::unwrapTable (::RPCServer::Table::Reader const & reader) {
    std::vector <std::string> header;
    std::vector <KeyTypes> meta;
    for (auto const & column : reader.getHeader()) header.emplace_back (column);
    for (auto const & info : reader.getMeta ()) meta.push_back (KeyTypes (info));
    Table table (header, meta);

    for (auto const & row : reader.getContent ()) {
        std::vector <Cell> newRow;
        for (auto const & field : row.getData()) {
            newRow.push_back (unwrapCell (field.getData()));
        }
        table.createRow (newRow);
    }
    return table;
}

::RPCServer::Table::Cell::Reader Wrapper::wrapCell (Cell const & cell) {
    ::capnp::MallocMessageBuilder builderBuilder;
    ::RPCServer::Table::Cell::Builder builder = builderBuilder.initRoot <::RPCServer::Table::Cell>();

    switch (cell.index ()) {
        case UNARY:
            builder.getData().setUnary();
            break;
        case BINARY:
            builder.getData().setBinary (std::get <bool> (cell));
            break;
        case SHORT:
            builder.getData().setShort (std::get <short> (cell));
            break;
        case LONG:
            builder.getData().setLong (std::get <long> (cell));
            break;
        case TEXT:
            builder.getData().setText (std::get <std::string> (cell));
            break;
        default:
            LOG (FATAL) << "Missing sanity check";
    }
    return builder.asReader();
}

Cell Wrapper::unwrapCell (::RPCServer::Table::Cell::Data::Reader const & field) {
    switch (field.which()) {
        case RPCServer::Table::Cell::Data::UNARY:
            return Cell();
        case RPCServer::Table::Cell::Data::BINARY:
            return Cell (field.getBinary());
        case RPCServer::Table::Cell::Data::SHORT:
            return Cell ((short) field.getShort());
        case RPCServer::Table::Cell::Data::LONG:
            return Cell ((long) field.getLong());
        case RPCServer::Table::Cell::Data::TEXT:
            return Cell (field.getText());
        default:
            LOG (FATAL) << "Missing sanity check";
    }
};

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */