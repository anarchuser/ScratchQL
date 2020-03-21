#ifndef DATABASE_PARSER_H
#define DATABASE_PARSER_H

#include "../Query/Query.h"

#include <kj/async.h>
#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <variant>

struct ParseTree {
private:
    ParseTree * inner = nullptr;
    ParseTree * next = nullptr;

public:
    std::string token;

    ParseTree * getInner();
    [[nodiscard]] ParseTree * tryGetInner() const;
    ParseTree * getNext();
    [[nodiscard]] ParseTree * tryGetNext() const;
    std::string getTokenName() const;
    [[nodiscard]] char operator [] (int index) const;
    ~ParseTree();
};

struct Parser {
    Parser() = delete;

    static kj::Own <Query> parseQuery (std::string const & rawQuery);
    static inline void validateQuery (std::string const & text);
    static inline void validateChar (char c);
    static inline bool isWordChar (char c);
    static std::string despace (std::string const & text);
    static std::string enrich (std::string const & text);
    static kj::Own <ParseTree> tokeniseQuery (std::string const & query);
    static void readToken (std::string::const_iterator * source, std::string::const_iterator end, ParseTree * tree);
    static kj::Own <Query> buildQuery (kj::Own <ParseTree> const & pt);
    static inline void copyToken (std::string::const_iterator * source, std::string & target);
    static inline short lookUpEnum (std::string const & str, std::vector <std::string> const & enums);

private:
//    static std::string
};

std::ostream & operator << (std::ostream & os, ParseTree * pt);

#endif //DATABASE_PARSER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */