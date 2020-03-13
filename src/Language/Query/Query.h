#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include "../../DBMS/Cell/Cell.h"

#include <string>
#include <variant>

namespace Database {
    enum Action { ADD, DELETE, IN };

    namespace Target {
        enum Type { TABLE, USER };
        enum Action { READ, WRITE, ADD, DELETE };

        namespace Table {
            enum Action { INSERT, REMOVE, UPDATE };

            struct Specification final {
                Action action;
                std::vector <std::size_t> row;
                std::vector <std::string> column;
            };
        }

        namespace User {
            enum Action { GIVE, TAKE };

            struct Specification final {
                Action action;
                Table::Specification target;
            };
        }

        using Specification = std::variant <Table::Specification, User::Specification>;
    }
}

struct Query final {
    Database::Action actionOnDatabase;
    std::string database;

    Database::Target::Type targetType;
    Database::Target::Action actionOnTarget;
    std::string target;

    Database::Target::Specification spec;
    std::vector <Cell> data;
};

#endif //DATABASE_QUERY_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */