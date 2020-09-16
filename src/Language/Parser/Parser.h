#ifndef DATABASE_PARSER_H
#define DATABASE_PARSER_H

#include "../Target/Target.h"
#include "ParseTree/ParseTree.h"

#include <kj/async.h>
#include <iostream>
#include <cmath>
#include <string>
#include <locale>
#include <vector>
#include <variant>

#define CLOSE_PAREN(c) (c == ')' || c == ']' || c == '}')
#define OPEN_PAREN(c)  (c == '(' || c == '[' || c == '{')
#define SEPARATOR(c)   (c == '.' || c == ',' || c == ':')

struct Parser {
    Parser() = delete;

    static Target parseQuery (std::string const & rawQuery);
    static inline void validateQuery (std::string const & text);
    static inline void validateChar (char c);
    static inline bool isWordChar (char c);
    static std::string despace (std::string const & text);
    static std::string enrich (std::string const & text);
    static kj::Own <ParseTree> tokeniseQuery (std::string const & query);
    static void readToken (std::string::const_iterator * source, std::string::const_iterator end, ParseTree * tree);
    static Target buildQuery (kj::Own <ParseTree> const & pt);
    static inline short lookUpEnum (std::string const & str, std::vector <std::string> const & enums);
    static void fillInSpecs (ParseTree const * tree);
    static void fillPairLists (ParseTree const * pt, std::vector <std::pair <std::string, Cell>> & list);
    static Cell tokenToCell (std::string const & token);
    static inline Cell strToNum (std::string const & token);
};

#endif //DATABASE_PARSER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */