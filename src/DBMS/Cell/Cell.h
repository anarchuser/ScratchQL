#ifndef DATABASE_CELL_H
#define DATABASE_CELL_H

#include "../../config.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <variant>

using Cell = std::variant <std::monostate, bool, short, long, std::string>;

enum CellType {
    UNARY = 0,
    BINARY,
    SHORT,
    LONG,
    TEXT,
};

bool operator ! (Cell const & cell);
std::ostream & operator << (std::ostream & os, Cell const & cell);
std::ostream & operator << (std::ostream & os, std::vector <Cell> const & cells);

std::string operator + (Cell const & cell);
std::string operator - (Cell const & cell);

template <class A, class B>
bool operator == (std::variant <A, B> first, std::variant <A, B> second) {
    return  first.index() == second.index() && first == second;}

template <class A, class B>
bool operator != (std::variant <A, B> first, std::variant <A, B> second) {
    return  first.index() != second.index() || !(first == second);}

bool operator == (Cell const & a, Cell const & b);
bool operator != (Cell const & a, Cell const & b);
bool operator <  (Cell const & a, Cell const & b);
bool operator >  (Cell const & a, Cell const & b);

bool isComparable (CellType a, CellType b);
void assertComparable (CellType a, CellType b);

Cell & toNull (Cell & cell);
const int CellLength[]= {0, 1, 6, 11}; //Defines number of reserved characters for each CellType

Cell writeToCell (std::string & inputString, CellType cellType);
std::string & cutTailingSpaces (std::string & content);

bool isValid (Cell const & cell);
bool isValid (Cell const * cell);

#endif //DATABASE_CELL_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */