#ifndef DATABASE_QUERY_ROW_H
#define DATABASE_QUERY_ROW_H

#include "Table.h"
#include "Column.h"
#include "Specification.h"
#include "../../Util/Helper.h"

#include <vector>
#include <utility>
#include <unordered_map>

namespace qy {
    struct Row {
        Row (Table parent, std::vector <Column> const & columnas, std::vector <Cell> const & data, std::vector <Specification> specs);

        Table const parent;
        std::unordered_map <std::string, Cell> const data;
        std::vector <Specification> const specs;

        bool operator == (Row const & other) const;
        bool operator != (Row const & other) const;
    };
}

std::ostream & operator << (std::ostream & os, qy::Row const & row);

#endif //DATABASE_QUERY_ROW_H

/* Copyright (C) 2020 Aaron Alef */