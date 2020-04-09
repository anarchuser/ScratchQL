#include "../main.h"
#include "../../src/DBMS/DBMS.h"

SCENARIO ("Issuing a query returns a valid Table") {
    GIVEN ("Some queries") {
        std::vector <std::string> queries {
                "CREATE(ParserTestDB)",
                "DELETE(ParserTestDB)",
                "CREATE(ParserTestDB)",
                "ParserTestDB.CREATE(@root)",
                "ParserTestDB.@root.GIVE()",
                "ParserTestDB.DELETE(@root)",
                "ParserTestDB.CREATE(#table, [name, surname, age])",
                "ParserTestDB.#table.READ ([name],[{surname:John},{age:10}])",
                "ParserTestDB.#root.INSERT([{name : Dee}, {surname : John}, {age : 10}])",
                "ParserTestDB.DELETE(#root)",
                "USERS()",
                "DATABASES()",
        };
        for (auto const & query : queries) {
            REQUIRE_NOTHROW (Parser::parseQuery (query));
            CHECK_NOTHROW (DBMS::evalQuery (query));
        }
    }

    GIVEN ("Some queries with return type") {
        std::vector <std::pair <std::string, ResponseType>> queries {
                {"create (TestDatabase)", VOID},
                {"", VOID},
                {"", VOID},
                {"", VOID},
                {"", VOID},
                {"", VOID},
                {"", VOID},
                {"", VOID},
                {"", VOID},
        };
        Response response;
        for (auto const & query : queries) {
            WHEN ("I evaluate a query with supposed return type " << ((query.second) ? "TABLE" : "NULL")) {
                CHECK_NOTHROW (response = DBMS::evalQuery (query.first));
                switch (query.second) {
                    case ResponseType::VOID:
                        THEN ("I get a NULL response") {
//                            CHECK (!response.index ());
                        }
                        break;
                    case ResponseType::TABLE:
                        THEN ("I get a valid Table as response") {
                            CHECK (!! * std::get <kj::Own <Table const>> (response));
                        }
                    default:
                        LOG (FATAL) << "ResponseType has gone insane";
                }
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */