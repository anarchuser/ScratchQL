#ifndef DATABASE_QUERY_COLUMN_H
#define DATABASE_QUERY_COLUMN_H

#include "Table.h"

#include <optional>

namespace qy {
    struct Column {
        Column (Table parent, std::string name, std::optional <Meta> meta = std::nullopt);

        std::string const name;
        Table const parent;
        std::optional <Meta> const meta;

        bool operator == (Column const & other) const;
        bool operator != (Column const & other) const;
    };
}

std::ostream & operator << (std::ostream & os, qy::Column const & col);

#endif //DATABASE_QUERY_COLUMN_H

/* Copyright (C) 2020 Aaron Alef */