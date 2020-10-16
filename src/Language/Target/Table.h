#ifndef DATABASE_QUERY_TABLE_H
#define DATABASE_QUERY_TABLE_H

#include "Database.h"
#include "../../DBMS/Table/Meta/Meta.h"

#include <utility>
#include <filesystem>
#include <optional>

namespace qy {
    struct Table {
        Table (Database parent, std::string name, std::optional <std::vector <Meta>> meta = std::nullopt);
        Table (std::string parent, std::string name, std::optional <std::vector <Meta>> meta = std::nullopt);
        Table (Table const & table, Meta meta);

        std::string const name;
        Database const parent;
        std::optional <std::vector <Meta>> const metae;

        std::filesystem::path const path;

        bool operator == (Table const & other) const;
        bool operator != (Table const & other) const;
    };
}

std::ostream & operator << (std::ostream & os, qy::Table const & table);

#endif //DATABASE_QUERY_TABLE_H

/* Copyright (C) 2020 Aaron Alef */