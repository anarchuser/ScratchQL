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
            LOG (FATAL) << "Cell printer has gone insane";
    }
}
std::string operator + (Cell const & cell) {
    std::stringstream ss;
    switch (cell.index()) {
        case UNARY:
            return "v";
        case BINARY:
            ss << "b'";
            break;
        case SHORT:
            ss << "s'";
            break;
        case LONG:
            ss << "l'";
            break;
        case TEXT:
            ss << "t'";
            break;
        default:
            LOG (FATAL) << "Cell pretty printer has gone insane";
    }
    ss << cell << '\'';
    return ss.str();
}

std::ostream & operator << (std::ostream & os, std::vector<Cell> const & cell) {
    for (auto const & key:cell){
        os << key << '\t';
    }
    return os;
}
void toNull (Cell & cell) {
    cell = Cell();
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */