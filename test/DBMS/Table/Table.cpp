#include "../../main.h"
#include "../../../src/DBMS/Table/Table.h"

SCENARIO ("I can create a table, modify and print it") {
    GIVEN ("Some test data") {
        std::string testdbname = "TableTestDatabase";
        std::string testtablename = "TableTestTable";
        CHECK_THROWS_AS (Table (std::vector <Meta> ()), std::range_error);
        CHECK_THROWS_AS (Table (std::vector <Meta> ({}, {})), std::range_error);

        std::vector <Meta> columns = {
                {"ID", SHORT, PRIMARY, true, false},
                {"Name", TEXT, NORMAL, false, false, 20},
                {"Birthday", TEXT, "Events", true, true, 10},
        };
        REQUIRE_NOTHROW (Table (columns).initDiskMode (testdbname, testtablename));
        Table t1 (columns);
        t1.initDiskMode (testdbname, testtablename);

        std::vector <std::vector <Cell>> rows {
                { short(1), std::string("John"), std::string("2020-02-02") },
                { short(2), std::string("Peter"), std::string("1929-03-20") },
                { short(3), std::string("Emily"), std::string("1978-10-31") },
                { short(4), std::string("Emily2"), std::string("1979-10-31") },
        };

        WHEN ("I add lines to it") {
            int row_counter = 0;
            for (auto const & row : rows) {
                CHECK (t1.getRowCount() == row_counter++);
                CHECK_NOTHROW (t1.createRow (row));
            }

            THEN ("The lines get added successfully") {
                CHECK (t1.getRowCount() == rows.size());
            }

            THEN ("I can successfully read lines") {
                auto const & t2 = t1;
                for (int i = 0; i < t2.getRowCount(); i++) {
                    CHECK_NOTHROW (t2.readRow (i));
                    CHECK_NOTHROW (t2.readRowAsVector (i));
                    CHECK (t2.readRowAsVector (i) == rows [i]);
                }
            }
//            THEN ("I can successfully read lines") {
//                for (int i = 0; i < t1.getRowCount(); i++) {
//                    CHECK_NOTHROW (t1.readRow (i));
//                    CHECK_NOTHROW (t1.readRowAsVector (i));
//                    CHECK (t1.readRowAsVector (i) == rows [i]);
//                }
//            }

            THEN ("Printing gives something meaningful") {
                std::stringstream ss;
                std::stringstream ts;
                for (auto const & col : columns) ts << col.name + "\t";
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
                    std::vector <Cell> testvector =  t1.readRowAsVector (i);
                    CHECK (t1.readRowAsVector (i) == rows [(i + 1) % rows.size()]);
                }
            }

            THEN ("I can successfully delete lines") {
                while (row_counter) {
                    CHECK (t1.getRowCount() == row_counter--);
                    CHECK_NOTHROW (t1.deleteRow (row_counter));
                }
                Table const & tc1 = t1;
                CHECK (!tc1.getRowCount());
                CHECK (!tc1);
            }
        }
        t1.deleteTable();
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */