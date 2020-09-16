#ifndef DATABASE_QUERY_ROW_H
#define DATABASE_QUERY_ROW_H

#include "Table.h"
#include "Column.h"
#include "Specification.h"

#include <vector>
#include <utility>

namespace qy {
    struct Row {
        Row (Table parent, std::vector <Column> columns, std::vector <Specification> specs);

        Table parent;
        std::vector <Column> columns;
        std::vector <Specification> specs;
    };
}

#endif //DATABASE_QUERY_ROW_H

/* Copyright (C) 2020 Aaron Alef */