#ifndef DATABASE_TIME_H
#define DATABASE_TIME_H

#include <chrono>

#define TIME(res,cmd) {                                         \
    auto __start__ = std::chrono::high_resolution_clock::now(); \
    res = cmd;                                                  \
    auto __stop__ = std::chrono::high_resolution_clock::now();  \
    LOG (INFO) << "The executed Command took " <<               \
    std::chrono::duration_cast <std::chrono::microseconds>      \
            (__stop__ - __start__).count() <<                   \
    " microseconds";} true

#endif //DATABASE_TIME_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */