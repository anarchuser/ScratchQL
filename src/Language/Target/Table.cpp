#include "Table.h"

qy::Table::Table (Database parent, std::string name) :
        name {std::move(name)},
        parent {std::move(parent)} {}

bool qy::Table::operator == (Table const & other) const {
    return  other.name   == name &&
            other.parent == parent;
}
bool qy::Table::operator != (Table const & other) const {
    return !(* this == other);
}

/* Copyright (C) 2020 Aaron Alef */