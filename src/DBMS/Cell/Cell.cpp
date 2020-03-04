#include "Cell.h"

bool operator ! (Cell const & cell) {
    return !cell.index();
}

std::ostream & operator << (std::ostream & os, Cell const & cell) {
    switch (cell.index()) {
        case UNARY:
            os << "v";
            break;
        case BINARY:
            os << "b'" << std::get <bool> (cell) << "'";
            break;
        case SHORT:
            os << "s'" << std::get <short> (cell) << "'";
            break;
        case LONG:
            os << "l'" << std::get <long> (cell) << "'";
            break;
        case TEXT:
            os << "t'" << std::get <std::string> (cell) << "'";
            break;
        default:
            LOG (FATAL) << "<< Cell has gone insane";
    }
    return os;
}

void toNull (Cell & cell) {
    cell = Cell();
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */