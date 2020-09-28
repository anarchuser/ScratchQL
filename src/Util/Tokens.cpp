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

std::string const & sv::checkName (std::string const & name) {
    if (name == META_DIR) {

    }
    if (name.empty()) {
        THROW (std::invalid_argument ("Name expected, got an empty string!"));
    }
    for (auto c : name) if (!std::isalnum (c)) {
        THROW (std::invalid_argument (STR+ "Expected valid name, got '" + name + "'!"));
    }
    return name;
}
std::string       & sv::checkName (std::string       & name) {
    checkName (std::as_const (name));
    return name;
}



/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */