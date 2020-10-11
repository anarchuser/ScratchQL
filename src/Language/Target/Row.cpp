#include "Row.h"

qy::Row::Row (Table parent, std::vector <Column> columns, std::vector <Cell> data, std::vector <Specification> specs) :
        parent {std::move (parent)},
        columns {std::move (columns)},
        data {std::move (data)},
        specs {std::move (specs)} {}

bool qy::Row::operator == (Row const & other) const {
    return  other.parent  == parent  &&
            other.columns == columns &&
            other.data    == data    &&
            other.specs   == specs;
}
bool qy::Row::operator != (Row const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Row const & row) {
    return os << "R(" << row.parent << ", " << row.columns << ", F(" << row.data << "), " << row.specs << ")";
}

/* Copyright (C) 2020 Aaron Alef */