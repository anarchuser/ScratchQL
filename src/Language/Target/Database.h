#ifndef DATABASE_QUERY_DATABASE_H
#define DATABASE_QUERY_DATABASE_H

#include "../../config.h"

#include <iostream>
#include <string>
#include <utility>
#include <filesystem>

namespace qy {
    struct Database {
        explicit Database (std::string name);

        std::string name;

        std::filesystem::path path;

        bool operator == (Database const & other) const;
        bool operator != (Database const & other) const;
    };
}

std::ostream & operator << (std::ostream & os, qy::Database const & db);

#endif //DATABASE_DATABASE_H

/* Copyright (C) 2020 Aaron Alef */