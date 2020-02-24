#include "../../main.h"

#include <string>

TEST_CASE ("I can store different data in Cells and modify and retrieve it") {
    bool Bool = true;
    long Long = 1234;
    double Double = 5678.9;
    std::string String = "Hello, World!";
    Cell BOOL (Bool);
    Cell LONG (Long);
    Cell DOUBLE (Double);
    Cell STRING (String);

//    std::cout << BOOL.index() << ": " << std::get <bool> (BOOL) << std::endl;
//    std::cout << LONG.index() << ": " << std::get <long> (LONG) << std::endl;
//    std::cout << DOUBLE.index() << ": " << std::get <double> (DOUBLE) << std::endl;
//    std::cout << STRING.index() << ": " << std::get <std::string> (STRING) << std::endl;

    CHECK (std::get <bool> (BOOL) == Bool);
    CHECK (std::get <long> (LONG) == Long);
    CHECK (std::get <double> (DOUBLE) == Double);
    CHECK (std::get <std::string> (STRING) == String);
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */