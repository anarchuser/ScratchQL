#include "../../main.h"
#include "../../../src/Language/Parser/Parser.h"

SCENARIO ("I can convert strings into meaningful Query structs (or errors)") {
    GIVEN ("Some correct queries") {
        std::vector <std::string> queries {
                "cReAtE (ParserTestDB)",
                "  delete        (  ParserTestDB  )    ",
                "CREATE(ParserTestDB)",
                "ParserTestDB.create (root)",
                "ParserTestDB.root.set(\"fnesiunfuisenfiusni9302*(%&£*(&%$*&£%\")",
                "ParserTestDB.delete (root)",
                "USERS ()",
                "DATABASES ()",
        };
        std::vector <std::string> despacedQueries {
                "cReAtE(ParserTestDB)",
                "delete(ParserTestDB)",
                "CREATE(ParserTestDB)",
                "ParserTestDB.create(root)",
                "ParserTestDB.root.set(\"fnesiunfuisenfiusni9302*(%&£*(&%$*&£%\")",
                "ParserTestDB.delete(root)",
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
                "$ParserTestDB.CREATE($root)",
                "$ParserTestDB.$root.SET(\"fnesiunfuisenfiusni9302*(%&£*(&%$*&£%\")",
                "$ParserTestDB.DELETE($root)",
                "USERS()",
                "DATABASES()",
        };
        for (std::size_t i = 0; i < queries.size(); i++) {
            CHECK (tokenisedQueries [i] == Parser::tokenise (despacedQueries [i]));
        }
    }

//        kj::Own <Query> queryResult;
//        CHECK_NOTHROW (queryResult = Parser::parseQuery (""));

}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */