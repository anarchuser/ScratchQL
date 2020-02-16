#include "Cell.h"

bool operator ! (Cell cell) {
    switch (cell.index()) {
        case 0:
            return std::get <bool> (cell) == 0;
        case 1:
            return std::get <long> (cell) == 0;
        case 2:
            return std::get <double> (cell) == 0.0;
        case 3:
            return std::get <std::string> (cell).empty();
        default:
            throw std::logic_error ("Fix 'Cell::operator !' !!!");
    }
}

void toNull (Cell & cell) {
    switch (cell.index()) {
        case 0:
            cell.emplace<bool>();
            break;
        case 1:
            cell.emplace<long>();
            break;
        case 2:
            cell.emplace<double>();
            break;
        case 3:
            cell.emplace<std::string>();
            break;
        default:
            throw std::logic_error ("Fix 'Cell::toNull' !!!");
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */