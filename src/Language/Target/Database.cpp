#include "Database.h"

qy::Database::Database (std::string name) :
        name {std::move (name)},
        path {DB_DIR/this->name} {}

bool qy::Database::operator == (Database const & other) const {
    return  other.path == path;
}
bool qy::Database::operator != (Database const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Database const & db) {
    return os << "D(" << "\"" << db.name << "\")";
}

/* Copyright (C) 2020 Aaron Alef */