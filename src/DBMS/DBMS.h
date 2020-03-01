#ifndef DATABASE_DBMS_H
#define DATABASE_DBMS_H

#include "Table/Table.h"

#include <memory>

struct DBMS {
    DBMS() = delete;

    static std::unique_ptr <Table> evalQuery (std::string const & query);
};


#endif //DATABASE_DBMS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */