#include <capnp/message.h>
#include "../../main.h"
#include "../../../src/Server/Wrapper/Wrapper.h"

#include "../../../src/Server/generated/ServerDBMS.capnp.h"
#include "../../../src/DBMS/Table/Table.h"

using namespace Wrapper;

TEST_CASE ("I can encode and decode Cells") {
    std::vector <Cell> cells {
        Cell (),
        Cell (false),
        Cell (true),
        Cell (short (10)),
        Cell (short (12345)),
        Cell (long (12)),
        Cell (long (12345676543)),
        Cell (1234l),
        Cell (std::string ("")),
        Cell (std::string ("fjesiofes")),
        Cell (std::string ("mfoiuesssnnnnnoijsoiejfposekjfpsoekjfpesojesjfiesj"))
    };
    std::vector <kj::Own <capnp::MallocMessageBuilder>> encodedCells;
    std::vector <Cell> decodedCells;

    for (auto const & cell : cells) {
        encodedCells.push_back (wrapCell (cell));
        decodedCells.push_back (unwrapCell (encodedCells.back()->getRoot <RPCServer::Table::Cell>()));
        CHECK (cell == decodedCells.back());
    }

    for (auto const & cell : cells) {
        capnp::MallocMessageBuilder builderBuilder;
        RPCServer::Table::Cell::Builder builder = builderBuilder.initRoot <::RPCServer::Table::Cell>();
        RPCServer::Table::Cell::Data::Builder builderData = builder.getData();

        switch (cell.index ()) {
            case UNARY:
                builderData.setUnary();
                break;
            case BINARY:
                builderData.setBinary (std::get <bool> (cell));
                break;
            case SHORT:
                builderData.setShort (std::get <short> (cell));
                break;
            case LONG:
                builderData.setLong (std::get <long> (cell));
                break;
            case TEXT:
                builderData.setText (std::get <std::string> (cell));
                break;
            default:
                LOG (FATAL) << "Test Cell Wrapper went insane: " << cell.index() << " not in range [0, 5)";
        }

        kj::Own <capnp::MallocMessageBuilder> cellBuilder = wrapCell (cell);
        LOG_ASSERT (cellBuilder.get());

        RPCServer::Table::Cell::Data::Reader data = cellBuilder->getRoot <RPCServer::Table::Cell>().asReader().getData();
        RPCServer::Table::Cell::Data::Reader reader = builderData.asReader();

        CHECK (cell == unwrapCell (builder.asReader()));
        CHECK (reader.which() == data.which());
        CHECK (reader.which() == cell.index());

        switch (data.which()) {
            case RPCServer::Table::Cell::Data::UNARY:
                break;
            case RPCServer::Table::Cell::Data::BINARY:
                CHECK (std::get <bool> (cell) == data.getBinary());
                break;
            case RPCServer::Table::Cell::Data::SHORT:
                CHECK (std::get <short> (cell) == data.getShort());
                break;
            case RPCServer::Table::Cell::Data::LONG:
                CHECK (std::get <long> (cell) == data.getLong());
                break;
            case RPCServer::Table::Cell::Data::TEXT:
                CHECK (std::get <std::string> (cell) == std::string (data.getText()));
                break;
            default:
                LOG (FATAL) << "Test Cell Unwrapper went insane: " << (short) data.which() << " not in range 0..4";
        }
    }
}

TEST_CASE ("I can encode and decode Tables") {
    std::vector <std::string> header {"surname", "name", "age", "profession"};
    std::vector <KeyTypes> meta {PRIMARY, PRIMARY, NORMAL, FOREIGN};
    std::vector <std::vector <Cell>> content {
        std::vector <Cell> {Cell (std::string ("Adam")), Cell (std::string ("Abcde")), Cell (short (12)), Cell()},
        std::vector <Cell> {Cell (std::string ("Eva")), Cell (std::string ("Vwxyz")), Cell (short (102)), Cell (std::string ("Teacher"))},
        std::vector <Cell> {Cell (std::string ("Tom")), Cell (std::string ("Oiuyt")), Cell (short (10)), Cell (std::string ("Kfefefsu"))},
        std::vector <Cell> {Cell (std::string ("Bob")), Cell (std::string ("Qwerty")), Cell (short (40)), Cell (std::string ("Engineer"))},
    };
    kj::Own <Table> initTable = kj::heap <Table> (header);
    for (auto const & row : content) initTable->createRow (row);

    Table table = kj::cp (* initTable);
    kj::Own <capnp::MallocMessageBuilder> tableBuilder = wrapTable (kj::mv (initTable));
    RPCServer::Table::Reader encodedTable = tableBuilder->getRoot <RPCServer::Table>();
    kj::Own <Table> decodedTable = unwrapTable (encodedTable);

    SECTION ("The original and processed table are both equal") {
        CHECK (table.getHeader()      == decodedTable->getHeader());
        CHECK (table.getColumnCount() == decodedTable->getColumnCount());
        CHECK (table.getRowCount()    == decodedTable->getRowCount());

        for (std::size_t row = 0; row < table.getRowCount(); row++) {
            for (auto const & col : table.getHeader()) {
                CHECK (table [col][row] == (* decodedTable) [col][row]);
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */