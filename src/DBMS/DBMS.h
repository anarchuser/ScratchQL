#ifndef DATABASE_DBMS_H
#define DATABASE_DBMS_H

#include "Table/Table.h"

#include <memory>

class DBMS {
public:
    DBMS () = default;

    std::unique_ptr <Table> operator () ();
};


#endif //DATABASE_DBMS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */