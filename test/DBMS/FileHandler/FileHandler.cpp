#include "../../main.h"
#include "../../../src/DBMS/FileHandler/FileHandler.h"

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
            CHECK_NOTHROW(fh->deleteDatabase());
        }
        SECTION ("We \"delete\" lines from a file") {
            REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable));
            for (auto &str : test_strings) fh->createLine(str);
            WHEN ("We delete one line with valid index") {
                int del_valid_index = 0;
                int strlength;
                THEN("It's characters are replaced with spaces") {
                    std::string del_string(fh->readLine(del_valid_index));
                    CHECK(!del_string.empty());
                    CHECK_NOTHROW(strlength = del_string.length());
                    fh->deleteLine(del_valid_index);
                    del_string = fh->readLine(del_valid_index);
                    CHECK (std::string(strlength, ' ') == del_string);
                }
            }
            WHEN ("We try to delete one line with invalid index") {
                int del_invalid_index = 100;
                std::string old_string;
                THEN("No changes are made to the line") {
                    std::string del_string(fh->readLine(del_invalid_index));
                    old_string = del_string;
                    CHECK(del_string.empty());
                    fh->deleteLine(del_invalid_index);
                    del_string = fh->readLine(del_invalid_index);
                    CHECK(old_string == del_string);
                }
            }
            CHECK_NOTHROW (fh->deleteDatabase());
        }
        SECTION("We update lines of a file"){
            REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable));
            for (auto &str : test_strings) fh->createLine(str);
            std::ifstream testfile (fh->path, std::ios::in);
            WHEN ("We update one line with valid index"){
                int replace_index = 0;
                int strlength;
                std::string target_string = "Goodbye, World - Sayonara";
                THEN("It's characters are replaced with a new string"){
                    std::string old_string (fh->readLine(replace_index));
                    CHECK(!old_string.empty());
                    CHECK_NOTHROW(strlength = old_string.length());
                    fh->updateLine(replace_index, target_string);
                    if (target_string.length() <= old_string.length()){
                        std::string new_string = fh->readLine(replace_index);
                        CHECK (target_string == new_string);
                    }
                    else {
                        std::string string_old_pos = fh->readLine(replace_index);
                        CHECK(string_old_pos == std::string (string_old_pos.length(), ' '));
                        std::string current_line;
                        std::string last_line;
                        while (getline(testfile, current_line)){
                            if (!current_line.empty()) last_line = current_line;
                        }
                        CHECK(target_string == last_line);
                    }
                }
            }
            WHEN ("We try to replace one line with invalid index"){
                int invalid_num = 100;
                std::string old_string;
                std::string new_string = "Goodbye, World - Sayonara";
                THEN("No changes are made to the line"){
                    std::string replaceable_string(fh->readLine(invalid_num));
                    old_string = replaceable_string;
                    CHECK(replaceable_string.empty());
                    fh->updateLine(invalid_num, new_string);
                    std::string current_line;
                    std::vector <std::string> newfile {};
                    while (getline (testfile, current_line)){
                        newfile.push_back(current_line);
                    }
                    CHECK(test_strings == newfile);
                    replaceable_string = fh->readLine(invalid_num);
                    CHECK(old_string == replaceable_string);
                    testfile.close();
                }
            }
            CHECK_NOTHROW (fh->deleteDatabase());
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