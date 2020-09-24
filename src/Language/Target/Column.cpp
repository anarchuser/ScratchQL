#include "Column.h"

qy::Column::Column (Table parent, std::string name) :
        name {std::move(name)},
        parent {std::move(parent)} {}

bool qy::Column::operator == (Column const & other) const {
    return  other.name   == name &&
            other.parent == parent;
}
bool qy::Column::operator != (Column const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Column const & col) {
    return os << "C(" << "\"" << col.name << "\", " << col.parent << ")";
}

/* Copyright (C) 2020 Aaron Alef */