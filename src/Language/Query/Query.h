#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include "../../DBMS/Cell/Cell.h"

#include <string>
#include <variant>

namespace Database {
    enum Action { CHANGE, CREATE, DELETE, DATABASES, USERS };
    static std::vector <std::string> const ActionStrings { "CHANGE", "CREATE", "DELETE", "DATABASES", "USERS" };

    namespace Target {
        enum Type { VOID, TABLE, USER };
        enum Action { READ, WRITE, CREATE, DELETE };
        static std::vector <std::string> const TypeStrings { "VOID", "TABLE", "USER" };
        static std::vector <std::string> const ActionStrings { "READ", "WRITE", "CREATE", "DELETE" };

        namespace Table {
            enum Action { INSERT, REMOVE, UPDATE };
            static std::vector <std::string> const ActionStrings { "INSERT", "REMOVE", "UPDATE" };

            struct Specification final {
                Action action;
                std::vector <std::size_t> row;
                std::vector <std::string> column;

                bool operator == (Specification const & other) const;
                bool operator != (Specification const & other) const;
            };
        }

        namespace User {
            enum Action { GIVE, TAKE };
            static std::vector <std::string> const ActionStrings { "GIVE", "TAKE" };

            struct Specification final {
                Action action;
                Table::Specification target;

                bool operator == (Specification const & other) const;
                bool operator != (Specification const & other) const;
            };
        }

        using Specification = std::variant <std::monostate, Table::Specification, User::Specification>;
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

    bool operator == (Query const & other) const;
};

#endif //DATABASE_QUERY_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */