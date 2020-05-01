#include "Tokens.h"


std::vector <std::string> sv::splitTokens (std::string const & line, char delimiter) {
    std::vector <std::string> tokens;
    std::size_t index = 0;
    for (char c : line) {
        if (c == delimiter) ++index;
        else tokens [index] += c;
    }
    return tokens;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */