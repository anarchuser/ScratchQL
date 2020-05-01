#ifndef DATABASE_TOKENS_H
#define DATABASE_TOKENS_H

#include <iostream>

#include <string>
#include <vector>

namespace sv {
std::vector <std::string> splitTokens (std::string line, char delimiter = '\t');
}


#endif //DATABASE_TOKENS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */