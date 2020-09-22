#include "Database.h"

qy::Database::Database (std::string name) :
        name {std::move(name)} {}

bool qy::Database::operator == (Database const & other) const {
    return other.name == name;
}
bool qy::Database::operator != (Database const & other) const {
    return !(* this == other);
}

/* Copyright (C) 2020 Aaron Alef */