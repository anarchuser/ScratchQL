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
std::string operator - (Cell const & cell) {
    std::stringstream ss;
    ss << cell;
    return ss.str();
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
Cell & toNull (Cell & cell) {
    return cell = Cell();
}

Cell writeToCell (std::string & inputString, CellType cellType){
    cutTailingSpaces(inputString);
    switch (cellType) {
        case CellType::UNARY:
            return Cell();
        case CellType::BINARY:
            return bool  (std::stoi (inputString));
        case CellType::SHORT:
            return short (std::stoi (inputString));
        case CellType::LONG:
            return long  (std::stoi (inputString));
        case CellType::TEXT:
            return inputString;
        default:
            LOG (FATAL) << "Wrong data type given!";
    }
}

std::string & cutTailingSpaces (std::string & content) {
    while (content.back() == ' ') content.pop_back();
    return content;
}

bool isValid (Cell const & cell) {
    if (cell.index() <= CellType::TEXT) return true;
    LOG (WARNING) << "The given cell is in an invalid state (idx: " << cell.index() << ")!";
    return false;
}
bool isValid (Cell const * cell) {
    if (cell) return isValid (* cell);
    THROW (std::logic_error ("Expected pointer to cell, got null pointer"));
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */