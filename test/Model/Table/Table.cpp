#include "../../main.h"
#include "../../../src/Model/Table/Table.h"

SCENARIO ("I can create a table and modify it") {
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
            std::vector <std::vector <Cell>> rows;
            std::vector <Cell> row1 {1l, std::string ("John"), std::string ("2020-02-02")};
            std::vector <Cell> row2 {2l, std::string ("Peter"), std::string ("1929-03-20")};
            std::vector <Cell> row3 {3l, std::string ("Emily"), std::string ("1978-10-31")};
            rows.push_back (row1);
            rows.push_back (row2);
            rows.push_back (row3);

            int row_counter = 0;
            for (auto & row : rows) {
                CHECK (t1.getRowCount() == row_counter++);
                CHECK_NOTHROW (t1.createRow (row));
            }

            THEN ("The lines get added successfully") {
                CHECK (t1.getRowCount() == rows.size ());
            }

            THEN ("I can successfully read lines") {
                for (int i = 0; i < rows.size(); i++) {
                    CHECK_NOTHROW (t1.readRow (i));
                    CHECK_NOTHROW (t1.readRowAsVector (i));
                    CHECK (t1.readRowAsVector (i) == rows [i]);
                }
            }

            THEN ("I can successfully modify lines") {
                for (int i = 0; i < rows.size(); i++) {
                    CHECK_NOTHROW (t1.updateRow (i, rows[(i + 1) % rows.size()]));
                    CHECK (t1.readRowAsVector (i) == rows [(i + 1) % rows.size()]);
                }
            }

            THEN ("I can successfully delete lines") {
                for (int i = 0; i < rows.size(); i++) {
                    CHECK_NOTHROW (t1.deleteRow (i));
                }
                CHECK (t1.getRowCount() == rows.size());
                t1.removePadding ();
                CHECK (t1.getRowCount() == 0);
            }
        }


    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */