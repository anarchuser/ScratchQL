#include "../main.h"
#include "../../src/DBMS/DBMS.h"

SCENARIO ("Issuing a query returns a valid Table") {
    Table table = * DBMS::evalQuery("");
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */