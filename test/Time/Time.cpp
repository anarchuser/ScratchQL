#include "../main.h"
#include "../../src/Time/Time.h"

unsigned long fibonacci (unsigned long n) {
    if (n <= 1) return n;
    unsigned long res = fibonacci (n - 1) + fibonacci (n - 2);
    return res;
}

TEST_CASE ("I can benchmark single function calls") {
    GIVEN ("A function and some parameters") {
        unsigned long test;
        TIME (test, fibonacci (25));
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */