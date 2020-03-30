#include "../main.h"
#include "../../src/Time/Time.h"

unsigned long fibonacci (unsigned long n) {
    if (n <= 1) return n;
    unsigned long res = fibonacci (n - 1) + fibonacci (n - 2);
    return res;
}

TEST_CASE ("I can benchmark single function calls") {
    GIVEN ("A function and some parameters") {

        std::chrono::nanoseconds duration;
        CHECK_NOTHROW (duration = Time::diff (& fibonacci, 25ul));
        CHECK (duration.count() > 1000);
        CHECK (duration.count() < 10000000000);
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */