#include "../main.h"
#include "../../src/DBMS/DBMS.h"

SCENARIO ("Issuing a query returns a valid Table") {
    Table table = * DBMS::evalQuery("");

    std::stringstream ss;
    CHECK_NOTHROW (ss << table);
    CHECK (& (ss << table) == & ss);
    CHECK (!!table);
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */