#ifndef DATABASE_PARSER_H
#define DATABASE_PARSER_H

#include "../DBMS/DBMS.h"

#include <memory>

class Parser {
private:
    DBMS dbms;

public:
    explicit Parser (DBMS const & dbms);

    std::unique_ptr <std::string> operator () (std::string const & query);
};


#endif //DATABASE_PARSER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */