#include "Table.h"

#include <utility>

qy::Table::Table (Database parent, std::string name, std::optional <std::vector <Meta>> meta) :
        name {std::move (name)},
        parent {std::move (parent)},
        metae {std::move (meta)},
        path {this->parent.path/this->name} {}

qy::Table::Table (std::string parent, std::string name, std::optional <std::vector <Meta>> meta) :
        Table (qy::Database (
                std::move (parent)),
                std::move (name),
                std::move (meta)) {}

bool qy::Table::operator == (Table const & other) const {
    return  other.name   == name &&
            other.parent == parent;
}
bool qy::Table::operator != (Table const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Table const & table) {
    return os << "T(" << "\"" << table.name << "\", " << table.parent << ")";
}

/* Copyright (C) 2020 Aaron Alef */