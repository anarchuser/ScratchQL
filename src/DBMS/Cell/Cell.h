#ifndef DATABASE_CELL_H
#define DATABASE_CELL_H

#include "../../config.h"

#include <stdexcept>
#include <string>
#include <variant>

using Cell = std::variant <std::monostate, bool, short, long, std::string>;

enum CellType {
    UNARY,
    BINARY,
    SHORT,
    LONG,
    TEXT,
};

bool operator ! (Cell const & cell);
std::ostream & operator << (std::ostream & os, Cell const & cell);
std::ostream & operator < (std::ostream & os, Cell const & cell);

template <class A, class B>
bool operator == (std::variant <A, B> first, std::variant <A, B> second) {
    return  first.index() == second.index() && first == second;}

template <class A, class B>
bool operator != (std::variant <A, B> first, std::variant <A, B> second) {
    return  !(first == second);}



void toNull (Cell & cell);

#endif //DATABASE_CELL_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */