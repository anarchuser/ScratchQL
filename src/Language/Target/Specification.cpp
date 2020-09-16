#include "Specification.h"

qy::Specification::Specification (Column column, Cell value, Predicate predicate) :
        column {std::move (column)},
        value {std::move (value)},
        predicate_e {predicate},
        predicate {bindPredicate (predicate, this->value)} {}

std::function <bool (Cell)> qy::Specification::bindPredicate (Predicate predicate, const Cell & value) {
    return [value, predicate](Cell const & cell) -> bool {
        switch (predicate) {
            case EQUALS:
                return cell == value;
            case UNEQUALS:
                return cell != value;
            case SMALLER:
                return cell < value;
            case BIGGER:
                return cell > value;
        }
    };
}

/* Copyright (C) 2020 Aaron Alef */