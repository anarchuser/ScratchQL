#ifndef DATABASE_QUERY_SPECIFICATION_H
#define DATABASE_QUERY_SPECIFICATION_H

#include "../../config.h"
#include "Column.h"

#include <functional>
#include <utility>

namespace qy {
    enum Predicate {EQUALS, UNEQUALS, SMALLER, BIGGER};

    struct Specification {
        Specification (Column column, Cell value, Predicate predicate);

        Column column;
        Cell value;
        Predicate predicate_e = EQUALS;
        std::function <bool (Cell)> predicate;

    private:
        static std::function <bool (Cell)> bindPredicate (Predicate predicate, Cell const & value);
    };
}


#endif //DATABASE_QUERY_SPECIFICATION_H

/* Copyright (C) 2020 Aaron Alef */