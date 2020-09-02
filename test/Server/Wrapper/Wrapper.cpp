#include <capnp/message.h>
#include "../../main.h"
#include "../../../src/Server/Wrapper/Wrapper.h"

#include "../../../src/Server/generated/ServerDBMS.capnp.h"
#include "../../../src/DBMS/Table/Table.h"
#include "../../../src/DBMS/Table/Meta/Meta.h"

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

        switch (cell.index ()) {
            case UNARY:
                builder.setUnary();
                break;
            case BINARY:
                builder.setBinary (std::get <bool> (cell));
                break;
            case SHORT:
                builder.setShort (std::get <short> (cell));
                break;
            case LONG:
                builder.setLong (std::get <long> (cell));
                break;
            case TEXT:
                builder.setText (std::get <std::string> (cell));
                break;
            default:
                LOG (FATAL) << "Test Cell Wrapper went insane: " << cell.index() << " not in range [0, 5)";
        }

        kj::Own <capnp::MallocMessageBuilder> cellBuilder = wrapCell (cell);
        LOG_ASSERT (cellBuilder.get());

        RPCServer::Table::Cell::Reader data = cellBuilder->getRoot <RPCServer::Table::Cell>().asReader();
        RPCServer::Table::Cell::Reader reader = builder.asReader();

        CHECK (cell == unwrapCell (builder.asReader()));
        CHECK (reader.which() == data.which());
        CHECK (reader.which() == cell.index());

        switch (data.which()) {
            case RPCServer::Table::Cell::UNARY:
                break;
            case RPCServer::Table::Cell::BINARY:
                CHECK (std::get <bool> (cell) == data.getBinary());
                break;
            case RPCServer::Table::Cell::SHORT:
                CHECK (std::get <short> (cell) == data.getShort());
                break;
            case RPCServer::Table::Cell::LONG:
                CHECK (std::get <long> (cell) == data.getLong());
                break;
            case RPCServer::Table::Cell::TEXT:
                CHECK (std::get <std::string> (cell) == std::string (data.getText()));
                break;
            default:
                LOG (FATAL) << "Test Cell Unwrapper went insane: " << (short) data.which() << " not in range 0..4";
        }
    }
}

TEST_CASE ("I can encode and decode Meta information") {
    std::vector <Meta> const header {
            {"surname", TEXT, PRIMARY, true, false},
            {"name", TEXT, PRIMARY, true, false},
            {"age", SHORT, NORMAL, true, false},
            {"profession", TEXT, "Professions", false, true},
    };
    for (auto const & col : header) {
        std::stringstream raw, proc;
        CHECK (raw  << col);
        CHECK (proc << unwrapMeta (wrapMeta (col)->getRoot <RPCServer::Table::Meta>().asReader()));
        CHECK (raw.str() == proc.str());
        CHECK (col == unwrapMeta (wrapMeta (col)->getRoot <RPCServer::Table::Meta>().asReader()));
    }
}

TEST_CASE ("I can encode and decode Tables") {
    std::vector <Meta> const header {
        {"surname", TEXT, PRIMARY, true, false},
        {"name", TEXT, PRIMARY, true, false},
        {"age", SHORT, NORMAL, true, false},
        {"profession", TEXT, "Professions", false, true},
    };
    std::vector <std::vector <Cell>> content {
        {Cell (std::string ("Adam")), Cell (std::string ("Abcde")), Cell (short (12)), Cell()},
        {Cell (std::string ("Eva")), Cell (std::string ("Vwxyz")), Cell (short (102)), Cell (std::string ("Teacher"))},
        {Cell (std::string ("Tom")), Cell (std::string ("Oiuyt")), Cell (short (10)), Cell (std::string ("Kfefefsu"))},
        {Cell (std::string ("Bob")), Cell (std::string ("Qwerty")), Cell (short (40)), Cell (std::string ("Engineer"))},
    };
    kj::Own <Table> initTable = kj::heap <Table> (header, std::string("hi"), std::string("there"));
    for (auto const & row : content) initTable->createRow (row);

    Table table = kj::cp (* initTable);
    kj::Own <capnp::MallocMessageBuilder> tableBuilder = wrapTable (kj::mv (initTable));
    RPCServer::Table::Reader encodedTable = tableBuilder->getRoot <RPCServer::Table>();
    kj::Own <Table> decodedTable = unwrapTable (encodedTable);

    SECTION ("The original and processed table are both equal") {
        CHECK (table.getHeader()      == decodedTable->getHeader());
        CHECK (table.columnCount() == decodedTable->columnCount());
        CHECK (table.rowCount()    == decodedTable->rowCount());

        for (std::size_t row = 0; row < table.rowCount(); row++) {
            for (auto const & col : table.getHeader()) {
                CHECK (table [col][row] == (* decodedTable) [col][row]);
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */