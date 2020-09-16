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
            //CHECK_NOTHROW (DBMS::query (query));
        }
    }

    GIVEN ("Some queries with return type") {
        std::vector <std::pair <std::string, bool>> queries {
                {"create (TestDatabase)", false},
                {"", false},
                {"", false},
                {"", false},
                {"", false},
                {"", false},
                {"", false},
                {"", false},
                {"", false},
        };
        Response response;
        for (auto const & query : queries) {
            WHEN ("I evaluate a query with supposed return type " << ((query.second) ? "TABLE" : "NULL")) {
                //CHECK_NOTHROW (response = DBMS::query (query.first));
                if (query.second) {
                    THEN ("I get a valid Table as response") {
                        REQUIRE (!!response);
                        CHECK (!! * response.value());
                    }
                }
                else {
                    THEN ("I get a NULL response") {
//                            CHECK (!response);
                    }
                }
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */