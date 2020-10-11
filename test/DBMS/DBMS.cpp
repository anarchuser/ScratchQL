#include "../main.h"
#include "../../src/DBMS/DBMS.h"

using namespace std;
namespace fs = std::filesystem;

SCENARIO ("Issuing queries works") {
    for (auto const & db : fs::directory_iterator (DB_DIR))
        fs::remove_all (db);
    REQUIRE (fs::is_empty (DB_DIR));

    qy::Database db ("TestDB");
    qy::Table table (db, "TestTable");
    qy::Column col1 (table, "TestCol1");
    qy::Column col2 (table, "TestCol2");
    qy::Specification spec1 (col1, Cell("value"), qy::Predicate::EQUALS);
    qy::Specification spec2 (col2, Cell(128), qy::Predicate::SMALLER);
    qy::Row row (table, vector {col1, col2}, std::vector {Cell("cell"), Cell(234)}, vector {spec1, spec2});

    GIVEN ("Different Create queries") {
        WHEN ("I create a database") {
            REQUIRE (!fs::exists (db.path));

            CHECK_NOTHROW (DBMS::create (db));
            CHECK_NOTHROW (DBMS::create (db));

            THEN ("The corresponding folder gets created") {
                CHECK (fs::exists (db.path));
                CHECK (fs::is_directory (db.path));
                CHECK (fs::is_empty (db.path));
            }
        }
        WHEN ("I create a table") {
            REQUIRE (!fs::exists (table.path));

            CHECK_NOTHROW (DBMS::create (table));

            THEN ("the Table gets created") {
                CHECK (fs::exists (table.path));
                CHECK (fs::is_directory (table.path));
                CHECK (fs::exists (table.path / META_DIR));
                CHECK (fs::is_directory (table.path / META_DIR));
                CHECK (fs::exists (table.path / INDEX_DIR));
                CHECK (fs::is_directory (table.path / INDEX_DIR));
            }
        }
        WHEN ("I create columns") {
            CHECK_NOTHROW (DBMS::create (col1));
            CHECK_NOTHROW (DBMS::create (col2));
            THEN ("Table and Columns get created") {
                // TODO: check if columns were deleted
            }
        }
        WHEN ("I create a row") {
            CHECK_NOTHROW (DBMS::create (row));
            THEN ("All columns are created") {
                // TODO: check if row was created
            }
        }
    }
    GIVEN ("Different Select queries") {
        WHEN ("I create a database") {
            THEN ("The corresponding folder gets created") {
                // TODO: check if folder was created
            }
        }
        WHEN ("I create a table") {
            THEN ("the Table gets created") {
                // TODO: check if folder was created
            }
        }
        WHEN ("I create columns") {
            THEN ("Table and Columns get created") {
            }
        }
        WHEN ("I create a row") {
            THEN ("All columns are created") {
            }
        }
    }
    GIVEN ("Different Modify queries") {
        WHEN ("I create a database") {
            THEN ("The corresponding folder gets created") {
                // TODO: check if folder was created
            }
        }
        WHEN ("I create a table") {
            THEN ("the Table gets created") {
                // TODO: check if folder was created
            }
        }
        WHEN ("I create columns") {
            THEN ("Table and Columns get created") {
            }
        }
        WHEN ("I create a row") {
            THEN ("All columns are created") {
            }
        }
    }
    GIVEN ("Different Insert queries") {
        WHEN ("I create a database") {
            THEN ("The corresponding folder gets created") {
                // TODO: check if folder was created
            }
        }
        WHEN ("I create a table") {
            THEN ("the Table gets created") {
                // TODO: check if folder was created
            }
        }
        WHEN ("I create columns") {
            THEN ("Table and Columns get created") {
            }
        }
        WHEN ("I create a row") {
            THEN ("All columns are created") {
            }
        }
    }
    GIVEN ("Different Remove queries") {
        WHEN ("I remove a database") {
            CHECK_NOTHROW (DBMS::remove (db));
            CHECK_NOTHROW (DBMS::remove (db));
            THEN ("The corresponding folder gets created") {
                // TODO: check if folder was deleted
            }
        }
        WHEN ("I remove a table") {
            THEN ("the Table gets created") {
                // TODO: check if folder was deleted
            }
        }
        WHEN ("I remove columns") {
            THEN ("Table and Columns get created") {
                // TODO: check if columns were deleted
            }
        }
        WHEN ("I remove a row") {
            THEN ("All columns are created") {
                // TODO: check if row was deleted
            }
        }
    }

    for (auto const & folder : fs::directory_iterator (DB_DIR))
        fs::remove_all (folder);
    REQUIRE (fs::is_empty (DB_DIR));
}


/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */