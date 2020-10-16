#ifndef DATABASE_TOKENS_H
#define DATABASE_TOKENS_H

#include "../config.h"

#include <string>
#include <vector>

namespace sv {
std::vector <std::string> splitTokens (std::string line, char delimiter = '\t');
std::string const & checkName (std::string const & name);
}


#endif //DATABASE_TOKENS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */