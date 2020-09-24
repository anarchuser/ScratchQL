#ifndef DATABASE_QUERY_SPECIFICATION_H
#define DATABASE_QUERY_SPECIFICATION_H

#include "../../config.h"
#include "Column.h"

#include <functional>
#include <utility>

namespace qy {
    enum Predicate {EQUALS, UNEQUALS, SMALLER, BIGGER, CUSTOM};

    struct Specification {
        Specification (Column column, Cell value, Predicate predicate);

        Column column;
        Cell value;
        Predicate predicate_e = EQUALS;
        std::function <bool (Cell)> predicate;

        bool operator == (Specification const & other) const;
        bool operator != (Specification const & other) const;

    private:
        static std::function <bool (Cell)> bindPredicate (Predicate predicate, Cell const & value);
    };
}

std::ostream & operator << (std::ostream & os, qy::Specification const & spec);
std::ostream & operator << (std::ostream & os, qy::Predicate const & pred);

#endif //DATABASE_QUERY_SPECIFICATION_H

/* Copyright (C) 2020 Aaron Alef */