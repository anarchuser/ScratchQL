#include "../main.h"
#include "../../src/DBMS/DBMS.h"

using namespace std;
namespace fs = std::filesystem;

SCENARIO ("Issuing queries works") {
    for (auto const & db : fs::directory_iterator (DB_DIR))
        fs::remove_all (db);
    REQUIRE (fs::is_empty (DB_DIR));

    std::vector <Meta> metae {
            Meta ("TestCol1", CellType::TEXT, KeyType::NORMAL, true, false),
            Meta ("TestCol2", CellType::LONG, KeyType::PRIMARY, true, false)
    };
    qy::Database db ("TestDB");
    qy::Table table (db, "TestTable", metae);
    std::vector <qy::Column> cols {
            qy::Column (table, Meta ("TestCol1", CellType::TEXT, KeyType::NORMAL, true, false)),
            qy::Column (table, Meta ("TestCol2", CellType::LONG, KeyType::PRIMARY, true, false))
    };
    qy::Specification spec1 (cols [0], Cell("value"), qy::Predicate::EQUALS);
    qy::Specification spec2 (cols [1], Cell(128), qy::Predicate::SMALLER);
    qy::Row row (table, cols, std::vector {Cell("cell"), Cell(234)}, vector {spec1, spec2});

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
                CHECK (fs::is_empty (table.path / META_DIR));
                CHECK (fs::exists (table.path / INDEX_DIR));
                CHECK (fs::is_directory (table.path / INDEX_DIR));
                CHECK (!fs::is_empty (table.path / INDEX_DIR));
            }
            THEN ("the indices get created") {
                for (auto const & meta : *table.metae)
                    LOG_ASSERT (fs::exists (table.path / INDEX_DIR / (STR + meta.name + ".idx")));
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
        WHEN ("I create a "
              "table") {
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