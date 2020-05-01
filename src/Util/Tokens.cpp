#include "Tokens.h"


std::vector <std::string> sv::splitTokens (std::string line, char delimiter) {
    std::vector <std::string> tokens;
    while(line.size()){
        int index = line.find (delimiter);
        if (index != std::string::npos) {
            tokens.push_back (line.substr (0, index));
            line = line.substr(index + 1);
            if (line.empty()) tokens.emplace_back();
        } else {
            tokens.push_back (line);
            line = "";
        }
    }
    return tokens;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */