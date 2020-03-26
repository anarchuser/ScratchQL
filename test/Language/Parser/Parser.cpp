#include "../../main.h"
#include "../../../src/Language/Parser/Parser.h"

SCENARIO ("I can convert strings into meaningful Query structs (or errors)") {
    GIVEN ("Some correct queries") {
        std::vector <std::string> queries {
                "cReAtE (ParserTestDB)\n",
                "  delete        (  ParserTestDB  )    ",
                "CREATE(ParserTestDB)\n",
                "ParserTestDB.create (@root)\n",
                "ParserTestDB.@root.give(\"fnesiunfuisenfiusni9302*(%&£*(&%$*&£%\")",
                "ParserTestDB.delete (@root)",
                "USERS ()",
                "DATABASES ()",
        };
        std::vector <std::string> despacedQueries {
                "cReAtE(ParserTestDB)",
                "delete(ParserTestDB)",
                "CREATE(ParserTestDB)",
                "ParserTestDB.create(@root)",
                "ParserTestDB.@root.give(\"fnesiunfuisenfiusni9302*(%&£*(&%$*&£%\")",
                "ParserTestDB.delete(@root)",
                "USERS()",
                "DATABASES()",
        };
        WHEN ("I remove all spaces and new-lines") {
            for (std::size_t i = 0; i < queries.size(); i++) {
                THEN ("The resulting query doesn't contain them anymore") {
                    CHECK (despacedQueries [i] == Parser::despace (queries [i]));
                }
            }
        }

        std::vector <std::string> enrichedQueries {
                "CREATE(ParserTestDB)",
                "DELETE(ParserTestDB)",
                "CREATE(ParserTestDB)",
                "ParserTestDB.CREATE(@root)",
                "ParserTestDB.@root.GIVE(\"fnesiunfuisenfiusni9302*(%&£*(&%$*&£%\")",
                "ParserTestDB.DELETE(@root)",
                "USERS()",
                "DATABASES()",
        };
        for (std::size_t i = 0; i < queries.size(); i++) {
            CHECK (enrichedQueries [i] == Parser::enrich (despacedQueries[i]));
        }

        std::vector <kj::Own <ParseTree>> tokenisedQueries;
        std::vector <kj::Own <Query>> builtQueries;
        for (auto const & enrichedQuery : enrichedQueries) {
            CHECK_NOTHROW (tokenisedQueries.push_back (Parser::tokeniseQuery (enrichedQuery)));
            CHECK_NOTHROW (builtQueries.push_back (Parser::buildQuery (tokenisedQueries.back())));
        }

        WHEN ("I check for the string representation") {
            for (std::size_t i = 0; i < queries.size(); i++) {
                THEN ("It equals the original enriched query") {
                    CHECK (enrichedQueries [i] == tokenisedQueries [i]->str());
                }
            }
        }

        CHECK (* builtQueries [0] == * builtQueries [2]);
        CHECK (builtQueries [0]->actionOnDatabase == Database::CREATE);
        CHECK (builtQueries [1]->actionOnDatabase == Database::DELETE);
        CHECK (builtQueries [3]->actionOnDatabase == Database::CHANGE);
        CHECK (builtQueries [4]->actionOnDatabase == Database::CHANGE);
        CHECK (builtQueries [5]->actionOnDatabase == Database::CHANGE);
        CHECK (builtQueries [6]->actionOnDatabase == Database::USERS);
        CHECK (builtQueries [7]->actionOnDatabase == Database::DATABASES);

        CHECK (builtQueries [0]->database == "ParserTestDB");
        CHECK (builtQueries [1]->database == "ParserTestDB");
        CHECK (builtQueries [3]->database == "ParserTestDB");
        CHECK (builtQueries [4]->database == "ParserTestDB");
        CHECK (builtQueries [5]->database == "ParserTestDB");

        CHECK (builtQueries [3]->targetType == Database::Target::USER);
        CHECK (builtQueries [4]->targetType == Database::Target::USER);
        CHECK (builtQueries [5]->targetType == Database::Target::USER);

        CHECK (builtQueries [3]->actionOnTarget == Database::Target::Action::CREATE);
        CHECK (builtQueries [4]->actionOnTarget == Database::Target::Action::SELECT);
        CHECK (builtQueries [5]->actionOnTarget == Database::Target::Action::DELETE);

        CHECK (builtQueries [3]->target == "root");
        CHECK (builtQueries [4]->target == "root");
        CHECK (builtQueries [5]->target == "root");

    }
    GIVEN ("Some incorrect queries") {
        std::vector <std::string> invalidQueries {
                "iufskhfzrz8yor387wy6rliyu39trp3[]]'#'4o#23574.,/.,/.,#'[]l",
                " ijseo f*$RF$(**$&(&$(*&$(&*$(",
                "$msoires",
                "*()",
                "-=-=-=-=-(",
               R"(""""")",
//                "",
//                "((){}",
        };
        for (auto const & invalidQuery : invalidQueries) {
            CHECK_THROWS_AS (Parser::parseQuery (invalidQuery), std::logic_error);
        }
    }
    std::vector <std::string> queries {
            "CREATE(ParserTestDB)",
            "DELETE(ParserTestDB)",
            "CREATE(ParserTestDB)",
            "ParserTestDB.CREATE(@root)",
            "ParserTestDB.@root.GIVE()",
            "ParserTestDB.DELETE(@root)",
            "ParserTestDB.CREATE(#root, [name, surname, age])",
            "ParserTestDB.#root.INSERT([{name : Dee}, {surname : John}, {age : 10}])",
            "ParserTestDB.DELETE(#root)",
            "USERS()",
            "DATABASES()",
    };

    for (auto const & query : queries) {
        kj::Own <Query> queryResult;
        CHECK_NOTHROW (queryResult = Parser::parseQuery (query));
    }
}

TEST_CASE ("All little helper functions do what they're supposed to") {
    SECTION ("Giving strings to 'Parser::tokenToCell returns the right Cell") {
        std::string const cs ("$");
        bool tb = true;
        short ts = 123;
        long tl = 5324643234542356786;
        std::vector <std::string> tokens {
            "",
            cs + std::to_string (tb),
            cs + std::to_string (ts),
            cs + std::to_string (tl),
            "87093",
            "oifiesfhesoifs",
        };
        std::vector <Cell> cells {
            Cell(),
            Cell (tb),
            Cell (ts),
            Cell (tl),
        };
        for (std::size_t idx = cells.size(); idx < tokens.size(); idx++) cells.emplace_back (tokens [idx]);

        std::vector <Cell> newCells;
        for (std::size_t idx = 0; idx < tokens.size(); idx++) {
            CHECK_NOTHROW (newCells.push_back (Parser::tokenToCell (tokens [idx])));
            CHECK (newCells.back().index() == std::min (idx, 4lu));
            CHECK (cells [idx] == newCells [idx]);
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */