#include "Target.h"

std::ostream & operator << (std::ostream & os, Target const & target) {
    switch (target.index()) {
        case qy::Target::DATABASE:
            return os << std::get <qy::Database> (target);
        case qy::Target::TABLE:
            return os << std::get <qy::Table> (target);
        case qy::Target::COLUMN:
            return os << std::get <qy::Column> (target);
        case qy::Target::ROW:
            return os << std::get <qy::Row> (target);
        default:
        THROW (std::logic_error ("Wrong target index - can't print"));
    }
}

/* Copyright (C) 2020 Aaron Alef */