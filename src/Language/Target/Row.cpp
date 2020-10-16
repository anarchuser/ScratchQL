#include "Row.h"

qy::Row::Row (Table parent, std::vector <Column> const & columns, std::vector <Cell> const & data, std::vector <Specification> specs) :
        parent {std::move (parent)},
        data ([columns, data]()->std::unordered_map <std::string, Cell> {
            std::unordered_map <std::string, Cell> map;
            if (columns.size() != data.size())
                THROW (std::logic_error ("amount of columns doesn't match amount of data"));
            for (std::size_t i = 0; i < data.size(); i++)
                map [columns [i].name] = data [i];
            return map;
        }()),
        specs {std::move (specs)} {}

bool qy::Row::operator == (Row const & other) const {
    return  other.parent  == parent  &&
            other.data    == data    &&
            other.specs   == specs;
}
bool qy::Row::operator != (Row const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Row const & row) {
    return os << "R(" << row.parent << ", d(" << row.data << "), " << row.specs << ")";
}

/* Copyright (C) 2020 Aaron Alef */