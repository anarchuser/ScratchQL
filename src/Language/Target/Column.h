#ifndef DATABASE_QUERY_COLUMN_H
#define DATABASE_QUERY_COLUMN_H

#include "Table.h"

namespace qy {
    struct Column {
        Column (Table parent, std::string name);

        std::string name;
        Table parent;

        bool operator == (Column const & other) const;
        bool operator != (Column const & other) const;
    };
}

std::ostream & operator << (std::ostream & os, qy::Column const & col);

#endif //DATABASE_QUERY_COLUMN_H

/* Copyright (C) 2020 Aaron Alef */