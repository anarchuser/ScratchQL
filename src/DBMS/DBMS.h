#ifndef DATABASE_DBMS_H
#define DATABASE_DBMS_H

#include "Table/Table.h"
#include "../Language/Query/Query.h"
#include "../Language/Parser/Parser.h"

#include <memory>
#include <kj/async.h>

struct DBMS {
    DBMS() = delete;

    static kj::Own <Table const> evalQuery (std::string const & rawQuery);
    static kj::Own <Table const> evalTableQuery (kj::Own <Query> const & query);
    static kj::Own <Table const> evalUserQuery (kj::Own <Query> const & query);
};


#endif //DATABASE_DBMS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */