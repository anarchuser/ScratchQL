#ifndef DATABASE_DBMS_H
#define DATABASE_DBMS_H

#include "Table/Table.h"
#include "../Language/Query/Query.h"
#include "../Language/Parser/Parser.h"

#include <memory>
#include <kj/async.h>
#include <variant>

using Response = std::variant <std::monostate, kj::Own <Table const>>;
enum ResponseType {
    VOID,
    TABLE
};
std::ostream & operator << (std::ostream & os, Response const & response);

struct DBMS {
    DBMS() = delete;

    static Response evalQuery (std::string const & rawQuery);
    static Response evalTableQuery (kj::Own <Query> const & query);
    static Response evalUserQuery (kj::Own <Query> const & query);
};


#endif //DATABASE_DBMS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */