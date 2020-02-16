#ifndef DATABASE_CELL_H
#define DATABASE_CELL_H

#include <stdexcept>
#include <string>
#include <variant>

#include <iostream>

using Cell = std::variant<bool, long, double, std::string>;

bool operator ! (Cell);

void toNull (Cell &);


#endif //DATABASE_CELL_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */