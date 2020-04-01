#include "../main.h"
#include "../../src/DBMS/DBMS.h"

SCENARIO ("Issuing a query returns a valid Table") {
    std::vector <std::string> queries {
        "create (TestDatabase)",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
    };
    Table table = * std::get <kj::Own <Table const>> (DBMS::evalQuery("USERS ()"));
//    CHECK (!!table);
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */