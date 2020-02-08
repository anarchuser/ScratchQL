#include "../../main.h"
#include "../../../src/Model/Table/Table.h"

SCENARIO ("I can create a table and fill it with data") {
    GIVEN ("Some test data") {
        std::vector <std::string> columns = {
                "ID",
                "Name",
                "Birthday"
        };
        std::vector <KeyTypes> keyTypes = {
                PRIMARY,
                FOREIGN,
                NORMAL
        };
        Table t1 (columns, keyTypes);
        REQUIRE_NOTHROW (Table (columns, keyTypes));

        WHEN ("I add lines to it") {
            std::vector <std::vector <cell>> rows;
            std::vector <cell> row1 {1l, cell ("John"), cell ("2020-02-02")};
            std::vector <cell> row2 {2l, cell ("Peter"), cell ("1929-03-20")};
            std::vector <cell> row3 {3l, cell ("Emily"), cell ("1978-10-31")};
            rows.push_back (row1);
            rows.push_back (row2);
            rows.push_back (row3);

            int row_counter = 0;
            for (auto & row : rows) {
                CHECK (t1.getRows() == row_counter++);
                CHECK_NOTHROW (t1.createRow (row));
            }

            THEN ("The lines get added successfully") {
                CHECK (t1.getRows() == rows.size ());
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */