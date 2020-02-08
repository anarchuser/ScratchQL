#include "../../main.h"
#include "../../../src/Model/Table/Table.h"

SCENARIO ("I can create a table and fill it with data") {
    GIVEN ("Some test data") {
        std::vector <std::string> columns = {
                "ID",
                "Name",
                "Birthday"
        };
        std::vector <KeyTypes> keyTypes = {
                PRIMARY,
                FOREIGN,
                NORMAL
        };
        Table t1 (columns, keyTypes);


    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */