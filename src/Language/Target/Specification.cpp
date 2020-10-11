#include "Specification.h"

qy::Specification::Specification (Column column, Cell value, Predicate predicate) :
        column {std::move (column)},
        value {std::move (value)},
        predicate_e {predicate},
        predicate {bindPredicate (predicate, this->value)} {}

bool qy::Specification::operator == (Specification const & other) const {
    return  other.column == column &&
            other.value  == value  &&
            other.predicate_e == predicate_e;
}
bool qy::Specification::operator != (Specification const & other) const {
    return !(* this == other);
}
std::ostream & operator << (std::ostream & os, qy::Specification const & spec) {
    return os << "S(" << spec.column << ", " << +spec.value << ", " << spec.predicate_e << ")";
}
std::ostream & operator << (std::ostream & os, qy::Predicate const & pred) {
    switch (pred) {
        case qy::Predicate::EQUALS:
            return os << "P(=)";
        case qy::Predicate::UNEQUALS:
            return os << "P(!)";
        case qy::Predicate::SMALLER:
            return os << "P(<)";
        case qy::Predicate::BIGGER:
            return os << "P(>)";
        default:
            return os << "P(?)";
    }
}

std::function <bool (Cell)> qy::Specification::bindPredicate (Predicate predicate, const Cell & value) {
    switch (predicate) {
        case EQUALS:
            return [value] (Cell const & cell) -> bool { return cell == value; };
        case UNEQUALS:
            return [value] (Cell const & cell) -> bool { return cell != value; };
        case SMALLER:
            return [value] (Cell const & cell) -> bool { return cell < value; };
        case BIGGER:
            return [value] (Cell const & cell) -> bool { return cell > value; };
        default:
            THROW (std::logic_error ("No Predicate found. Initialise manually"));
    }
}

/* Copyright (C) 2020 Aaron Alef */