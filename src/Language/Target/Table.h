#ifndef DATABASE_QUERY_TABLE_H
#define DATABASE_QUERY_TABLE_H

#include "Database.h"

#include <utility>
#include <filesystem>

namespace qy {
    struct Table {
        Table (Database parent, std::string name);
        Table (std::string parent, std::string name);

        std::string name;
        Database parent;

        std::filesystem::path path;

        bool operator == (Table const & other) const;
        bool operator != (Table const & other) const;
    };
}

std::ostream & operator << (std::ostream & os, qy::Table const & table);

#endif //DATABASE_QUERY_TABLE_H

/* Copyright (C) 2020 Aaron Alef */