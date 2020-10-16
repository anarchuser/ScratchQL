#include "DBMS.h"

std::ostream & operator << (std::ostream & os, Response const & response) {
    if (response)
        os << * response.value ();
    return os;
}

void  DBMS::create (Target const & target) {
    switch (target.index()) {
        case qy::Target::DATABASE:
            FileHandler::create (std::get <qy::Database> (target));
            break;
        case qy::Target::TABLE:
            Table (std::get <qy::Table> (target));
            break;
        case qy::Target::COLUMN:
            FileHandler::create (std::get <qy::Column> (target));
            break;
        case qy::Target::ROW:
            insert (std::get <qy::Row> (target));
            break;
    }
}
Table DBMS::select (Target const & target) {
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
    Table table ("testDB", "testTable", header);
    for (auto const & row : content) table.createRow (row);
    return table;
}
void  DBMS::modify (qy::Row const & target) {
}
void  DBMS::insert (qy::Row const & target) {
    Table (target.parent).createRow(target.data);
}
void  DBMS::remove (Target const & target) {
    switch (target.index()) {
        case qy::Target::DATABASE:
            FileHandler::remove (std::get <qy::Database> (target));
            break;
        case qy::Target::TABLE:
            FileHandler::remove (std::get <qy::Table> (target));
            break;
        case qy::Target::COLUMN:
            FileHandler::remove (std::get <qy::Column> (target));
            break;
        case qy::Target::ROW:
            qy::Row const & row = std::get <qy::Row> (target);
            Table (row.parent).deleteRow (row.data);
            break;
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */