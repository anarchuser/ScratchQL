#include "../../main.h"
#include "../../../src/DBMS/Table/Table.h"

SCENARIO ("I can create a table, modify and print it") {
    GIVEN ("Some test data") {
        CHECK_THROWS_AS (Table (std::vector <std::string> ()), std::range_error);
        CHECK_THROWS_AS (Table (std::vector <std::string> ({"", ""})), std::range_error);

        std::vector <std::string> columns = {
                "ID",
                "Name",
                "Birthday"
        };
        Table t1 (columns);
        REQUIRE_NOTHROW (Table (columns));

        std::vector <std::vector <Cell>> rows;
        std::vector <Cell> row1 {1l, std::string ("John"), std::string ("2020-02-02")};
        std::vector <Cell> row2 {2l, std::string ("Peter"), std::string ("1929-03-20")};
        std::vector <Cell> row3 {3l, std::string ("Emily"), std::string ("1978-10-31")};
        rows.push_back (row1);
        rows.push_back (row2);
        rows.push_back (row3);

        WHEN ("I add lines to it") {
            CHECK (!t1);
            int row_counter = 0;
            for (auto const & row : rows) {
                CHECK (t1.getRowCount() == row_counter++);
                CHECK_NOTHROW (t1.createRow (row));
                CHECK (!!t1);
            }

            THEN ("The lines get added successfully") {
                CHECK (t1.getRowCount() == rows.size());
            }

            THEN ("I can successfully read lines") {
                for (int i = 0; i < t1.getRowCount(); i++) {
                    CHECK_NOTHROW (t1.readRow (i));
                    CHECK_NOTHROW (t1.readRowAsVector (i));
                    CHECK (t1.readRowAsVector (i) == rows [i]);
                }
            }

            THEN ("Printing gives something meaningful") {
                std::stringstream ss;
                std::stringstream ts;
                for (auto const & col : columns) ts << col + "\t";
                ts << "\n\n";

                for (auto const & row : rows) {
                    for (auto const & cell : row) {
                        ts << cell << "\t";
                    }
                    ts << "\n";
                }

                CHECK (& (ss << t1) == & ss);
                CHECK (ss.str() == ts.str());

            }

            THEN ("I can successfully modify lines") {
                for (int i = 0; i < t1.getRowCount(); i++) {
                    CHECK_NOTHROW (t1.updateRow (i, rows[(i + 1) % rows.size()]));
                    CHECK (t1.readRowAsVector (i) == rows [(i + 1) % rows.size()]);
                    CHECK (!!t1);
                }
            }

            THEN ("I can successfully delete lines") {
                while (row_counter) {
                    CHECK (!!t1);
                    CHECK (t1.getRowCount() == row_counter--);
                    CHECK_NOTHROW (t1.deleteRow (row_counter));
                }
                Table const tc1 (t1);
                CHECK (!t1.getRowCount());
                CHECK (!t1);

                CHECK (!tc1.getRowCount());
                CHECK (!tc1);
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */