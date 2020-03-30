#ifndef DATABASE_TIME_H
#define DATABASE_TIME_H

#include <chrono>

namespace Time {
    template <class Acc = std::chrono::microseconds, class R, class ... Args>
    Acc diff (R * result, R (function) (Args && ...), Args && ... arguments) {
        auto start = std::chrono::high_resolution_clock::now();
        * result = (!result) ?: function (arguments ...);
        auto stop = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast <Acc> (stop - start);
    };

    template <class Acc = std::chrono::microseconds, class R, class ... Args>
    Acc diff (R (function) (Args ...), Args && ... arguments) {
        auto start = std::chrono::high_resolution_clock::now();
        function (arguments ...);
        auto stop = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast <Acc> (stop - start);
    }
};


#endif //DATABASE_TIME_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */