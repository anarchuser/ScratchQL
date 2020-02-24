#include "../../main.h"
#include "../../../src/Model/FileHandler/FileHandler.h"

SCENARIO ("Reading from and writing to a file is executed correctly") {
    GIVEN ("A file and some test strings") {
        std::string test_path = PROJECT_ROOT + "/tmp/fileHandlerTest.tsv";
        std::string testdb = "testdb";
        std::string testtable = "testtable";
        FileHandler * fh;

        std::vector<std::string> test_strings = {
                "Hello, world!",
                "1234567890fghij",
                "I'm a test",
                "abcde",
                "vwxyz"
        };
        SECTION("We create database and table with the filehandler"){
            FileHandler *fhbadtest;
            std::string badname = "!&nogooddbname";
            WHEN("Database and table name are valid"){
                THEN("The constructor creates Folders for database and table and the table file"){
                    REQUIRE_NOTHROW(fhbadtest = new FileHandler(testdb, testtable));
                }
            }
            WHEN("We delete the Database"){
                THEN("The filehandler removes the directory of the database with all subdirectories"){
                    REQUIRE_NOTHROW(fhbadtest->deleteDatabase());
                }
            }
            WHEN("The database name is invalid"){
                THEN("The constructor throws an exception"){
                    REQUIRE_THROWS_AS(fhbadtest = new FileHandler(badname, testtable), std::invalid_argument);
                };
            }
            fhbadtest->deleteDatabase();
            WHEN("The table name is invalid"){
                THEN("The constructor throws an exception"){
                    REQUIRE_THROWS_AS(fhbadtest = new FileHandler(testdb, badname), std::invalid_argument);
                };
            }
            fhbadtest->deleteDatabase();
        }
        SECTION ("We read lines from a file") {
            REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable));
            WHEN ("We append lines to a file") {
                for (auto const & str : test_strings) {
                    REQUIRE_NOTHROW (fh->createLine (str));
                }
                THEN ("We can successfully read them") {
                    int i = 0;
                    for (auto const & str : test_strings) {
                        CHECK (fh->readLine (i++) == str);
                    }
                }
            }
            fh->deleteDatabase();
        }
        SECTION ("We \"delete\" lines from a file") {
            REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable));
            WHEN ("We delete one line"){
                for (auto & str : test_strings) fh->createLine(str);
                int del_index = 0;
                int strlength;
                THEN("It's characters are replaced with spaces"){
                    std::string del_string (fh->readLine(del_index));
                    CHECK(del_string != "");
                    CHECK_NOTHROW(strlength = del_string.length());
                    fh->deleteLine(del_index);
                    del_string = fh->readLine(del_index);
                    CHECK (std::string (strlength, ' ') == del_string);
                }
            }
        }

        SECTION ("A file can be deleted - twice") {
            CHECK_NOTHROW (fh->deleteTable());
            CHECK_NOTHROW (fh->deleteTable());
            CHECK_NOTHROW (fh->deleteDatabase());
            CHECK_NOTHROW (fh->deleteDatabase());
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */