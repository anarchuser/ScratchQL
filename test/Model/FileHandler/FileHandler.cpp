#include "../../main.h"
#include "../../../src/Model/FileHandler/FileHandler.h"

SCENARIO ("Reading from and writing to a file is executed correctly") {
    GIVEN ("A file and some test strings") {
        std::string test_path = PROJECT_ROOT + "/tmp/fileHandlerTest.tsv";
        FileHandler fh (test_path);

        std::vector<std::string> test_strings = {
                "Hello, World!",
                "jifjesoijfesoifj",
                "test",
                "",
                "xyz\rabc"
        };

        SECTION ("We read lines from a file") {
            WHEN ("The file does not exist") {
                FileHandler fh_tmp ("\r\r\r.\r");
                THEN ("The reading throws an error") {
                    fh_tmp.deleteTable();
                    CHECK_THROWS_AS (fh_tmp.readLine (0), std::ios_base::failure);
                }
            }

            WHEN ("We append lines to a file") {
                for (auto & str : test_strings) {
                    REQUIRE_NOTHROW (fh.createLine (str));
                }
                THEN ("We can successfully read them") {
                    int i = 0;
                    for (auto & str : test_strings) {
                        CHECK (fh.readLine (i++) == str);
                    }
                }
            }
        }

        SECTION ("A file can be deleted - twice") {
            CHECK_NOTHROW (fh.deleteTable());
            CHECK_NOTHROW (fh.deleteTable());
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */