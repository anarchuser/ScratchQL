#include "../../main.h"

#include <iostream>
#include <string>

TEST_CASE ("I can store different data in Cells and modify and retrieve it") {
    bool Bool = true;
    short Short = 1234;
    long Long = 5678;
    std::string String = "Hello, World!";
    Cell BOOL (Bool);
    Cell SHORT (Short);
    Cell LONG (Long);
    Cell STRING (String);


    std::stringstream ss;
    CHECK_NOTHROW (ss << +BOOL << std::endl);
    CHECK_NOTHROW (ss << +SHORT << std::endl);
    CHECK_NOTHROW (ss << +LONG << std::endl);
    CHECK_NOTHROW (ss << +STRING << std::endl);

    CHECK (std::get <bool> (BOOL) == Bool);
    CHECK (std::get <short> (SHORT) == Short);
    CHECK (std::get <long> (LONG) == Long);
    CHECK (std::get <std::string> (STRING) == String);
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */