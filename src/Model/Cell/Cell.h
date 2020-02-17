#ifndef DATABASE_CELL_H
#define DATABASE_CELL_H

#include <stdexcept>
#include <string>
#include <variant>

using Cell = std::variant <std::monostate, bool, long, double, std::string>;

bool operator ! (const Cell & cell);
std::ostream & operator << (std::ostream & os, const Cell & cell);

void toNull (Cell & cell);

#endif //DATABASE_CELL_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */