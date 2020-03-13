#include "DBMS.h"

kj::Own <Table const> DBMS::evalQuery (std::string const & query) {
    //TODO: Everything regarding processing of queries goes here.



    /* TEST IMPLEMENTATION. REMOVE AFTER SUCCESSFUL QUERY EXECUTION */
    auto testTable = kj::heap <Table> (std::vector <std::string> {"surname", "name", "age", "profession"}, std::vector <KeyTypes> {PRIMARY, PRIMARY, INDEXED, NORMAL});
    testTable->createRow(std::vector <Cell> {std::string ("Adam"), std::string ("Abcd"), short (30), Cell()});
    testTable->createRow(std::vector <Cell> {std::string ("Tom"),  std::string ("Efgh"), short (30), std::string ("jfuesfeoies")});
    testTable->createRow(std::vector <Cell> {std::string ("Eve"),  std::string ("Ijkl"), short (30), std::string ("nfwiufew")});
    testTable->createRow(std::vector <Cell> {std::string ("Dora"), std::string ("Mnop"), short (30), std::string ("jfuesfeoies")});

    return testTable;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */