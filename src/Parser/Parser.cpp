#include "Parser.h"

Parser::Parser (DBMS const & dbms) : dbms{dbms} {}

std::unique_ptr <std::string> Parser::operator () (std::string const & query) {}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */