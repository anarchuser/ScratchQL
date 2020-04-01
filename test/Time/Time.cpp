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

        WHEN ("I log calculating the 25th Fibonacci number recursively") {
            LOG_TIME (test = fibonacci (25));

            THEN ("It takes a few milliseconds to execute") {
                CHECK (test > 100);
                CHECK (test < 100000);
            }
        }

        WHEN ("I calculate the 25th Fibonacci number recursively") {
            test = TIME (fibonacci (25)).count();

            THEN ("It takes a few milliseconds to execute") {
                CHECK (test > 100);
                CHECK (test < 100000);
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */