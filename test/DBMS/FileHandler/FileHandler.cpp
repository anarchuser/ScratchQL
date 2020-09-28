#include "../../main.h"
#include "../../../src/DBMS/FileHandler/FileHandler.h"

#include <algorithm>
#include <memory>
#include <unordered_map>

SCENARIO ("I create, modify and delete databases") {
    GIVEN ("Database and Table names") {
        std::vector <std::string> db_names {
                "First",
                "Second",
                "Third"
        };
        std::vector <std::vector <std::string>> temp_table_names {
                {"first", "second", "third"},
                {"alpha", "beta", "gamma", "delta"},
                {"su", "mo", "tu", "we", "th", "fr", "sa"}
        };
        std::vector <std::size_t> lengths {1, 6, 11, 15};
        std::vector <CellType> types {CellType::BINARY, CellType::SHORT, CellType::LONG, CellType::TEXT};

        std::unordered_map <std::string, std::vector <std::string>> table_names;
        for (std::size_t idx = 0; idx < db_names.size(); idx++) {
            table_names.insert ({db_names [idx], std::move (temp_table_names [idx])});
        }

        WHEN ("I create tables") {
            for (auto & db : db_names) {
                for (auto & table : table_names [db]) {
                    CHECK_NOTHROW (FileHandler (db, table, lengths, types));
                }
            }
            THEN ("I can delete them again") {
                WHEN ("I delete some tables only") {
                    for (auto & db : db_names) {
                        CHECK_NOTHROW (FileHandler (db, table_names[db][0], lengths, types).deleteTable ());
                    }
                    THEN ("The others stay untouched") {
                        // TODO: Check that other tables are still there
                    }
                }
                WHEN ("I delete databases") {
                    for (auto & db : db_names) {
                        CHECK_NOTHROW (FileHandler (db, table_names[db][1], lengths, types).deleteDatabase ());
                    }
                    THEN ("The others stay untouched") {
                        // TODO: Check that other tables are still there
                    }
                }
            }
        }
    }
    GIVEN ("A database filled with some strings") {
        std::string testdb = "testdb";
        std::string testtable = "testtable";
        std::vector <std::size_t> colLengths = {15, 15, 10, 6, 5};
        std::vector <CellType> cellTypes (colLengths.size(), CellType::TEXT);

        std::vector <std::vector <Cell>> test_strings {
                {"Hello, world!", "1234567890fghij", "I'm a test", "abcde", "vwxyz"},
                {"whatever", "fjsefesefesfesf", "qwerty", "[]'[#/;{;][#[", "()"},
                {"monday", "tuesday", "wednesday", "thursday", "friday"},
                {"bbbb", "ccc", "dd", "e", ""},
        };

        std::unique_ptr <FileHandler> fh;
        REQUIRE_NOTHROW(fh = std::make_unique <FileHandler> (testdb, testtable, colLengths, cellTypes));

        WHEN ("I add lines to the table") {
//            for (auto const & strings : test_strings) {
//                CHECK_NOTHROW (fh->createLine (strings));
//            }
//            THEN("The lines get appended to the table file") {
//                 //TODO: Check if they actually get appended (size check should suffice)
//            }
//            for (auto & list : test_strings) {
//                std::transform (list.cbegin(), list.cend(), colLengths.cbegin(),
//                                list.begin(), [](Cell const & str, std::size_t size) -> Cell {
//                    return (-str).substr (0, size);
//                });
//            }
//            THEN ("I can read the lines from the file") {
//                std::size_t idx = 0;
//                for (auto const & strings : test_strings) {
//                    std::size_t stridx = 0;
//                    for (auto const & cell : fh->readLine (idx++)) {
//                        CHECK (cell == strings [stridx++]);
//                    }
//                }
//            }
//            WHEN ("I update the lines") {
//                std::size_t idx = test_strings.size();
//                for (auto const & strings : test_strings) {
//                    CHECK_NOTHROW (fh->updateLine (--idx, strings));
//                    THEN ("Reading them results in the updated version") {
//                        std::size_t stridx = 0;
//                        for (auto const & cell : fh->readLine (idx++)) {
//                            CHECK (cell == strings [stridx++]);
//                        }
//                    }
//                }
//            }
//            THEN ("I can 'delete' the lines") {
//                std::size_t idx = test_strings.size();
//                while (idx --> 0) {
//                    CHECK (fh->readLine (idx - 1).size());
//                    fh->deleteLine (idx - 1);
//
//                    auto deleted_strings = fh->readLine (idx - 1);
//                    for (auto const & deleted : deleted_strings){
//                        CHECK(deleted == Cell(std::string()));
//                    }
//                }
//            }
        }
//
//        // TODO: Add proper mechanism to check if line index is valid
//
//            WHEN ("I use clearLines()"){
//                std::string tmpline;
//                std::size_t counter_before = 0, counter_after = 0;
//                THEN ("Empty lines are removed"){
//                    while (getline (testfile, tmpline)) counter_before++;
//                    testfile.close();
//                    CHECK_NOTHROW (fh-> deleteLine(2));
//                    CHECK_NOTHROW (fh-> clearLines());
//                    testfile.open(fh->path, std::ios::in);
//                    while (getline (testfile, tmpline)){
//                        CHECK (tmpline.length() == fh->lineLength);
//                        counter_after++;
//                    }
//                    testfile.close();
//                    CHECK (counter_before > counter_after);
//                    CHECK (counter_after > 1);
//                }
//            }

//        CHECK_NOTHROW (fh->deleteTable());
//        CHECK_NOTHROW (fh->deleteTable());
//        CHECK_NOTHROW (fh->deleteDatabase());
//        CHECK_NOTHROW (fh->deleteDatabase());
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */