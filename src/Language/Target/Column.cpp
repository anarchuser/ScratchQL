#include "Column.h"

qy::Column::Column (Table parent, std::string name, std::optional <Meta> meta) :
        name {std::move (name)},
        parent {std::move (parent)},
        meta {std::move (meta)} {}

bool qy::Column::operator == (Column const & other) const {
    return  other.parent == parent &&
            other.meta   == meta;
}
bool qy::Column::operator != (Column const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Column const & col) {
    return os << "C(" << "\"" << col.name << "\", M(" << col.meta << "), " << col.parent << ")";
}

/* Copyright (C) 2020 Aaron Alef */