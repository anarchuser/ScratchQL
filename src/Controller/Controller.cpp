#include "Controller.h"

std::string mock (const char * input, size_t len) {
    printf("%s\n", input);
    return std::string ("Hello, World!");
}

/* Copyright (C) 2020 Aaron Alef */