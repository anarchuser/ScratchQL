#ifndef DATABASE_QUERY_TABLE_H
#define DATABASE_QUERY_TABLE_H

#include "Database.h"

#include <utility>

namespace qy {
    struct Table {
        Table (Database parent, std::string name);

        std::string name;
        Database parent;
    };
}


#endif //DATABASE_QUERY_TABLE_H

/* Copyright (C) 2020 Aaron Alef */