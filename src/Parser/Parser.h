#ifndef DATABASE_PARSER_H
#define DATABASE_PARSER_H

#include "../DBMS/DBMS.h"

#include <memory>

namespace Parser {
    std::unique_ptr <std::string> parseQuery (std::string const & query);
};


#endif //DATABASE_PARSER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */