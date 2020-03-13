#ifndef DATABASE_DBMS_H
#define DATABASE_DBMS_H

#include "Table/Table.h"
#include "../Language/Query/Query.h"

#include <memory>
#include "kj/async.h"

struct DBMS {
    DBMS() = delete;

    static kj::Own <Table const> evalQuery (std::string const & query);
};


#endif //DATABASE_DBMS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */