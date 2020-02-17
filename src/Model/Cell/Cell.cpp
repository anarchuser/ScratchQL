#include "Cell.h"

bool operator ! (const Cell & cell) {
    return !cell.index();
}

std::ostream & operator << (std::ostream & os, const Cell & cell) {
    std::visit([& os] (auto const & val) {os << val;}, cell);
    return os;
}

void toNull (Cell & cell) {
    cell = Cell();
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */