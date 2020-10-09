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

std::ostream & operator << (std::ostream & os, std::vector <Cell> const & cells) {
    os << '[';
    bool first = true;
    for (auto const & cell : cells) {
        if (!first) os << ", ";
        os << +cell;
        first = false;
    }
    return os << ']';
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

bool operator == (Cell const & a, Cell const & b) {
    if (!isComparable((CellType) a.index(), (CellType) b.index())) return false;
    switch (a.index ()) {
        case UNARY:
            return true;
        case BINARY:
            return std::get <bool> (a) == std::get <bool> (b);
        case SHORT:
        case LONG:
            return  ((a.index() == SHORT) ? std::get <short> (a) : std::get <long> (b)) ==
                    ((b.index() == SHORT) ? std::get <short> (b) : std::get <long> (b));
        case TEXT:
            return std::get <std::string> (a) == std::get <std::string> (b);
        default:
            LOG (FATAL) << "This can't be happening";
    }
}
bool operator != (Cell const & a, Cell const & b) {
    return !(a == b);
}
bool operator < (Cell const & a, Cell const & b) {
    assertComparable ((CellType) a.index(), (CellType) b.index());
    switch (a.index ()) {
        case UNARY:
            return false;
        case BINARY:
            return std::get <bool> (a) < std::get <bool> (b);
        case SHORT:
        case LONG:
            return  ((a.index() == SHORT) ? std::get <short> (a) : std::get <long> (b)) <
                    ((b.index() == SHORT) ? std::get <short> (b) : std::get <long> (b));
        case TEXT:
            return std::get <std::string> (a).size() < std::get <std::string> (b).size();
        default:
            LOG (FATAL) << "This can't be happening";
    }
}
bool operator > (Cell const & a, Cell const & b) {
    assertComparable ((CellType) a.index(), (CellType) b.index());
    switch (a.index ()) {
        case UNARY:
            return false;
        case BINARY:
            return std::get <bool> (a) > std::get <bool> (b);
        case SHORT:
        case LONG:
            return  ((a.index() == SHORT) ? std::get <short> (a) : std::get <long> (b)) >
                    ((b.index() == SHORT) ? std::get <short> (b) : std::get <long> (b));
        case TEXT:
            return std::get <std::string> (a).size() > std::get <std::string> (b).size();
        default:
            LOG (FATAL) << "This can't be happening";
    }
}

bool isComparable (CellType a, CellType b) {
    return (a == b || ((a == SHORT && b == LONG) || (a == LONG && b == SHORT)));
}
void assertComparable (CellType a, CellType b) {
    if (!isComparable (a, b))
        THROW (std::logic_error (STR+
                "The given types, " + std::to_string(a) + " and " + std::to_string(b) + ", aren't comparable!"));
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */