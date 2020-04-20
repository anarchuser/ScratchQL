#include "Cell.h"

bool operator ! (Cell const & cell) {
    return !cell.index();
}

std::ostream & operator << (std::ostream & os, Cell const & cell) {
    switch (cell.index()) {
        case UNARY:
            return os;
        case BINARY:
            return (os << std::get <bool> (cell));
        case SHORT:
            return (os << std::get <short> (cell));
        case LONG:
            return (os << std::get <long> (cell));
        case TEXT:
            return (os << std::get <std::string> (cell));
        default:
            LOG (FATAL) << "<< Cell has gone insane";
    }
}
std::ostream & operator < (std::ostream & os, Cell const & cell) {
    switch (cell.index()) {
        case UNARY:
            return (os << "v");
        case BINARY:
            os << "b'";
            break;
        case SHORT:
            os << "s'";
            break;
        case LONG:
            os << "l'";
            break;
        case TEXT:
            os << "t'";
            break;
        default:
            LOG (FATAL) << "<<= Cell has gone insane";
    }
    return os << cell << "'";
}

void toNull (Cell & cell) {
    cell = Cell();
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */