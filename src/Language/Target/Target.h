#ifndef DATABASE_QUERY_TARGET_H
#define DATABASE_QUERY_TARGET_H

#include "Database.h"
#include "Table.h"
#include "Column.h"
#include "Row.h"

#include <variant>

typedef std::variant <qy::Database, qy::Table, qy::Column, qy::Row> Target;

namespace qy {
    enum Target {DATABASE, TABLE, COLUMN, ROW};
}

#endif //DATABASE_QUERY_TARGET_H

/* Copyright (C) 2020 Aaron Alef */