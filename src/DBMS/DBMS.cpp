#include "DBMS.h"

#define COLUMNS 7
#define ROWS 18
#define VALUE

kj::Own <Table const> DBMS::evalQuery (std::string const & query) {
    //TODO: Everything regarding processing of queries goes here.
    // MAKE FUNCTIONAL!!!!

    /* TEST IMPLEMENTATION. REMOVE AFTER SUCCESSFUL QUERY EXECUTION */
    auto testTable = kj::heap <Table> (std::vector <std::string> {"surname", "name", "age", "profession"}, std::vector <KeyTypes> {PRIMARY, PRIMARY, INDEXED, NORMAL});
    testTable->createRow(std::vector <Cell> {std::string ("Adam"), std::string ("Abcd"), short (30), Cell()});
    testTable->createRow(std::vector <Cell> {std::string ("Tom"),  std::string ("Efgh"), short (30), std::string ("jfuesfeoies")});
    testTable->createRow(std::vector <Cell> {std::string ("Eve"),  std::string ("Ijkl"), short (30), std::string ("nfwiufew")});
    testTable->createRow(std::vector <Cell> {std::string ("Dora"), std::string ("Mnop"), short (30), std::string ("jfuesfeoies")});

    auto & table = * testTable;

    for (auto const & col : table.getHeader()) std::cout << col [0] << "\t";
    std::cout << std::endl;

    for (auto const & col : table.getMeta()) std::cout << (int) col << "\t";
    std::cout << std::endl;
    std::cout << std::endl;

    for (size_t row = 0; row < table.getRowCount(); row++) {
        for (auto const & cell : table.readRowAsVector (row)) {
            std::cout << (short) cell.index () << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return testTable;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */