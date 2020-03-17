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
            CHECK (tokenisedQueries [i] == Parser::tokenise (despacedQueries [i]));
        }

        std::vector <kj::Own <Query>> builtQueries;
        for (auto const & tokenisedQuery : tokenisedQueries) {
            CHECK_NOTHROW (builtQueries.push_back (Parser::buildQuery (tokenisedQuery)));
        }
        CHECK (* builtQueries [0] == * builtQueries [2]);


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