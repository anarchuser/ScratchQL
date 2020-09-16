#include "DBMS.h"

void  DBMS::create (Target const & target) {
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
    Table table (header, "testDB", "testTable");
    for (auto const & row : content) table.createRow (row);
    return table;
}
void  DBMS::modify (qy::Row const & target, std::vector <Cell> const & data) {
}
void  DBMS::insert (qy::Table const & target, std::vector <Cell> const & data) {
}
void  DBMS::remove (Target const & target) {
}

std::ostream & operator << (std::ostream & os, Response const & response) {
    if (response)
        os << * response.value();
    return os;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */