#include "../../main.h"
#include "../../../src/Language/Parser/Parser.h"

SCENARIO ("I can convert strings into meaningful Query structs (or errors)") {
    GIVEN ("Some correct queries") {
        std::vector <std::string> queries {
                "cReAtE (ParserTestDB)",
                "  delete        (  ParserTestDB  )    ",
                "CREATE(ParserTestDB)",
                "ParserTestDB.create (@root)",
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
        for (std::size_t i = 0; i < queries.size(); i++) {
            CHECK (despacedQueries [i] == Parser::despace (queries [i]));
        }

        std::vector <std::string> tokenisedQueries {
                "CREATE($ParserTestDB)",
                "DELETE($ParserTestDB)",
                "CREATE($ParserTestDB)",
                "$ParserTestDB.CREATE($@root)",
                "$ParserTestDB.$@root.GIVE(\"fnesiunfuisenfiusni9302*(%&£*(&%$*&£%\")",
                "$ParserTestDB.DELETE($@root)",
                "USERS()",
                "DATABASES()",
        };
        for (std::size_t i = 0; i < queries.size(); i++) {
            CHECK (tokenisedQueries [i] == Parser::enrich (despacedQueries[i]));
        }

        std::vector <kj::Own <Query>> builtQueries;
        for (auto const & tokenisedQuery : tokenisedQueries) {
            CHECK_NOTHROW (builtQueries.push_back (Parser::buildQuery (tokenisedQuery)));
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
        CHECK (builtQueries [4]->actionOnTarget == Database::Target::Action::CHANGE);
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

//        kj::Own <Query> queryResult;
//        CHECK_NOTHROW (queryResult = Parser::parseQuery (""));

}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */