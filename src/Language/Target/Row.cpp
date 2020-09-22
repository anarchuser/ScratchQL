#include "Row.h"

qy::Row::Row (Table parent, std::vector <Column> columns, std::vector <Specification> specs) :
        parent {std::move(parent)},
        columns {std::move(columns)},
        specs {std::move(specs)} {}

bool qy::Row::operator == (Row const & other) const {
    return  other.parent  == parent &&
            other.columns == columns &&
            other.specs   == specs;
}
bool qy::Row::operator != (Row const & other) const {
    return !(* this == other);
}

/* Copyright (C) 2020 Aaron Alef */