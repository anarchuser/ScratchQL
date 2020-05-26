#include "../../main.h"
#include "../../../src/DBMS/FileHandler/FileHandler.h"

SCENARIO ("Reading from and writing to a file is executed correctly") {
    GIVEN ("A file and some test strings") {
        std::string test_path = PROJECT_ROOT + "/tmp/fileHandlerTest.tsv";
        std::string testdb = "testdb";
        std::string testtable = "testtable";
        std::vector <int> const testColLength = {25};
        std::vector <Cell> testContent = {"HappyBirthday"};
        std::vector <CellType> testCellType = {CellType::TEXT};
        FileHandler * fh;

        std::vector <std::vector <Cell>> test_strings1 = {
                {"Hello, world!",
                "1234567890fghij",
                "I'm a test",
                "abcde",
                "vwxyz"}
        };
        std::vector <int> const testColLengths1 = {15, 15, 10, 6, 5};
        std::vector <CellType> testCellTypes1 = {CellType::TEXT, CellType::TEXT, CellType::TEXT, CellType::TEXT, CellType::TEXT};
        SECTION("We create database and table with the filehandler"){
            FileHandler * fhbadtest;
            std::string badname = "!&nogooddbname";
            WHEN("Database and table name are valid"){
                THEN("The constructor creates Folders for database and table and the table file"){
                    REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable, testColLength, testCellType));
                }
            }
            WHEN("We delete the Database"){
                THEN("The filehandler removes the directory of the database with all subdirectories"){
                    REQUIRE_NOTHROW(fh->deleteDatabase());
                }
            }
            WHEN("The database name is invalid"){
                THEN("The constructor throws an exception"){
                    REQUIRE_THROWS_AS(fhbadtest = new FileHandler(badname, testtable, testColLength, testCellType), std::invalid_argument);
                };
            }
            WHEN("The table name is invalid"){
                THEN("The constructor throws an exception"){
                    REQUIRE_THROWS_AS(fhbadtest = new FileHandler(testdb, badname, testColLength, testCellType), std::invalid_argument);
                };
            }
        }
        SECTION ("We read lines from a file") {
            REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable, testColLengths1, testCellTypes1));
            WHEN ("We append lines to a file") {
                for (std::vector <Cell> & str : test_strings1) {
                    REQUIRE_NOTHROW (fh->createLine (str));
                }
                THEN ("We can successfully read them") {
                    int i = 0;
                    for (std::vector <Cell> const & str : test_strings1) {
                        CHECK (fh->readLine (i++) == str);
                    }
                }
            }
            CHECK_NOTHROW(fh->deleteDatabase());
        }
        SECTION ("We \"delete\" lines from a file") {
            REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable, testColLengths1, testCellTypes1));
            for (std::vector <Cell> & str : test_strings1) fh->createLine(str);
            WHEN ("We delete one line with valid index") {
                int del_valid_index = 0;
                THEN("It's characters are replaced with spaces") {
                    std::vector <Cell> del_string = fh->readLine(del_valid_index);
                    CHECK(!del_string.empty());
                    fh->deleteLine(del_valid_index);
                    del_string = fh->readLine(del_valid_index);
                    CHECK (del_string.size() == testColLengths1.size());
                    for (Cell tooLongCell : del_string){
                        CHECK(tooLongCell == Cell(""));
                    }
                }
            }
            //This Test assumes readLine on an invalid index returns nothing, which is not true (yet, May 18th 2020)
//            WHEN ("We try to delete one line with invalid index") {
//                int del_invalid_index = 100;
//                std::vector <Cell> old_string;
//                THEN("No changes are made to the line") {
//                    std::vector <Cell> del_string = fh->readLine(del_invalid_index);
//                    old_string = del_string;
//                    CHECK(del_string.empty());
//                    fh->deleteLine(del_invalid_index);
//                    del_string = fh->readLine(del_invalid_index);
//                    CHECK(del_string == old_string);
//                }
//            }
            CHECK_NOTHROW (fh->deleteDatabase());
        }
        SECTION("We update lines of a file"){
            REQUIRE_NOTHROW(fh = new FileHandler(testdb, testtable, testColLengths1, testCellTypes1));
            for (std::vector <Cell> & str : test_strings1) fh->createLine(str);
            std::ifstream testfile (fh->path, std::ios::in);
            WHEN ("We update one line with valid index"){
                int replace_index = 0;
                int strlength;
                std::vector <Cell> target_string = {"Hello, world!", "1234567890fghij",
                            "I'm a test", "abcde", "vwxyz"};
                THEN("It's characters are replaced with a new string"){
                    std::vector <Cell> old_string = fh->readLine(replace_index);
                    CHECK(!old_string.empty());
//                    CHECK_NOTHROW(strlength = calcLineLength(old_string));
                    fh->updateLine(replace_index, target_string);
                    std::vector <Cell> new_string = fh->readLine(replace_index);
                    CHECK (new_string == target_string);
                }
            }
//            WHEN ("We try to replace one line with invalid index"){
//                int invalid_num = 100;
//                std::string old_string;
//                std::string new_string = "Goodbye, World - Sayonara";
//                THEN("No changes are made to the line"){
//                    std::vector <Cell> replaceable_string(fh->readLine(invalid_num));
//                    std::cerr << replaceable_string << std::endl;
//                    REQUIRE(replaceable_string.empty());
//                    old_string = replaceable_string;
//                    fh->updateLine(invalid_num, new_string);
//                    std::string current_line;
//                    std::vector <Cell> newfile {};
//                    while (getline (testfile, current_line)){
//                        fh->cutTailingSpaces(current_line);
//                        newfile.push_back(current_line);
//                    }
//                    CHECK(test_strings1 == newfile);
//                    replaceable_string = fh->readLine(invalid_num);
//                    CHECK(replaceable_string == old_string);
//                    testfile.close();
//                }
//            }
//            WHEN ("We use clearLines()"){
//                std::string tmpline;
//                int counter_before = 0, counter_after = 0;
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
            CHECK_NOTHROW (fh->deleteDatabase());
        }
//                std::filesystem::copy_file(fh->path, "/home/felix/Desktop/table.tsv",
//                                           std::filesystem::copy_options::overwrite_existing);

        SECTION ("A file can be deleted - twice") {
            CHECK_NOTHROW (fh->deleteTable());
            CHECK_NOTHROW (fh->deleteTable());
            CHECK_NOTHROW (fh->deleteDatabase());
            CHECK_NOTHROW (fh->deleteDatabase());
        }
    }
}


/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */