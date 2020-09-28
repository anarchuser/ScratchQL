#include "Database.h"

qy::Database::Database (std::string name) :
        name {std::move(name)},
        path {PROJECT_ROOT/this->name} {}

bool qy::Database::operator == (Database const & other) const {
    return other.name == name;
}
bool qy::Database::operator != (Database const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Database const & db) {
    return os << "D(" << "\"" << db.name << "\")";
}

/* Copyright (C) 2020 Aaron Alef */