#include <capnp/message.h>
#include "../../main.h"
#include "../../../src/Server/Wrapper/Wrapper.h"
#include "../../../src/Language/Target/Target.h"

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
        encodedCells.push_back (wrap (cell));
        decodedCells.push_back (unwrap (encodedCells.back()->getRoot <RPCServer::Table::Cell>()));
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

        kj::Own <capnp::MallocMessageBuilder> cellBuilder = wrap (cell);
        LOG_ASSERT (cellBuilder.get());

        RPCServer::Table::Cell::Reader data = cellBuilder->getRoot <RPCServer::Table::Cell>().asReader();
        RPCServer::Table::Cell::Reader reader = builder.asReader();

        CHECK (cell == unwrap (builder.asReader()));
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
        CHECK (proc << unwrap (wrap (col)->getRoot <RPCServer::Table::Meta>().asReader()));
        CHECK (raw.str() == proc.str());
        CHECK (col == unwrap (wrap (col)->getRoot <RPCServer::Table::Meta>().asReader()));
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
    kj::Own <Table> table = kj::heap <Table> (header, std::string("hi"), std::string("there"));
    for (auto const & row : content) table->createRow (row);

    kj::Own <capnp::MallocMessageBuilder> tableBuilder = wrap (* table);
    RPCServer::Table::Reader encodedTable = tableBuilder->getRoot <RPCServer::Table>();
    kj::Own <Table> decodedTable = unwrap (encodedTable);

    SECTION ("The original and processed table are both equal") {
        CHECK (table->getHeader()   == decodedTable->getHeader());
        CHECK (table->columnCount() == decodedTable->columnCount());
        CHECK (table->rowCount()    == decodedTable->rowCount());

        for (std::size_t row = 0; row < table->rowCount(); row++) {
            for (auto const & col : table->getHeader()) {
                CHECK ((* table) [col][row] == (* decodedTable) [col][row]);
            }
        }
    }
}

TEST_CASE ("I can encode and decode Queries") {
    std::vector <Cell> data {Cell (151), Cell ("hello")};

    qy::Database db ("db");
    qy::Table table (db, "table");
    qy::Column col1 (table, "col1");
    qy::Column col2 (table, "col2");
    qy::Specification spec1 (col1, data [0], qy::Predicate::BIGGER);
    qy::Specification spec2 (col2, data [1], qy::Predicate::UNEQUALS);
    qy::Row row (table, std::vector <qy::Column> {col1, col2}, data, std::vector <qy::Specification> {spec1, spec2});

    SECTION ("Databases serialisation") {
        Target original = Target (db);
        auto message = Wrapper::wrap (original);
        RPCServer::Target::Reader encoded = message->getRoot <RPCServer::Target>().asReader();

        Target decoded = Wrapper::unwrap (encoded);
        CHECK (decoded.index() == original.index());
        CHECK (decoded.index() == 0);
        CHECK (+ decoded == + original);
        CHECK (decoded == original);

        auto got = std::get <qy::Database> (decoded);
        CHECK (got == db);
    }
    SECTION ("Table serialisation") {
        Target original = Target (table);
        auto message = Wrapper::wrap (original);
        RPCServer::Target::Reader encoded = message->getRoot <RPCServer::Target>().asReader();
        Target decoded = Wrapper::unwrap (encoded);

        CHECK (decoded.index() == original.index());
        CHECK (decoded.index() == 1);
        CHECK (+ decoded == + original);
        CHECK (decoded == original);

        auto got = std::get <qy::Table> (decoded);
        CHECK (got == table);
    }
    SECTION ("Column1 serialisation") {
        Target original = Target (col1);
        auto message = Wrapper::wrap (original);
        RPCServer::Target::Reader encoded = message->getRoot <RPCServer::Target>().asReader();
        Target decoded = Wrapper::unwrap (encoded);

        CHECK (decoded.index() == original.index());
        CHECK (decoded.index() == 2);
        CHECK (+ decoded == + original);
        CHECK (decoded == original);

        auto got = std::get <qy::Column> (decoded);
        CHECK (got == col1);
    }
    SECTION ("Column2 serialisation") {
        Target original = Target (col2);
        auto message = Wrapper::wrap (original);
        RPCServer::Target::Reader encoded = message->getRoot <RPCServer::Target>().asReader();
        Target decoded = Wrapper::unwrap (encoded);

        CHECK (decoded.index() == original.index());
        CHECK (decoded.index() == 2);
        CHECK (+ decoded == + original);
        CHECK (decoded == original);

        auto got = std::get <qy::Column> (decoded);
        CHECK (got == col2);
    }
    SECTION ("Row serialisation") {
        Target original = Target (row);
        auto message = Wrapper::wrap (original);
        RPCServer::Target::Reader encoded = message->getRoot <RPCServer::Target>().asReader();
        Target decoded = Wrapper::unwrap (encoded);
        std::vector <qy::Specification> specs = Wrapper::unwrap (encoded.getRow().getSpecs());

        CHECK (decoded.index() == original.index());
        CHECK (decoded.index() == 3);
        CHECK (+ decoded == + original);
        CHECK (decoded == original);

        auto got = std::get <qy::Row> (decoded);
        CHECK (got == row);
        CHECK (row.specs == specs);
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */