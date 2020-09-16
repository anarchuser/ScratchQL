#ifndef DATABASE_QUERY_COLUMN_H
#define DATABASE_QUERY_COLUMN_H

#include "Table.h"

namespace qy {
    struct Column {
        Column (Table parent, std::string name);

        std::string name;
        Table parent;

    };
}


#endif //DATABASE_QUERY_COLUMN_H

/* Copyright (C) 2020 Aaron Alef */