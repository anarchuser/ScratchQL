#ifndef DATABASE_CELL_H
#define DATABASE_CELL_H

#include "../../config.h"

#include <stdexcept>
#include <string>
#include <variant>

using Cell = std::variant <std::monostate, bool, short, long, std::string>;

enum CellTypes {
    UNARY,
    BINARY,
    SHORT,
    LONG,
    TEXT,
};

bool operator ! (Cell const & cell);
std::ostream & operator << (std::ostream & os, Cell const & cell);

void toNull (Cell & cell);

#endif //DATABASE_CELL_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */