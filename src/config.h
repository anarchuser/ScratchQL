#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

#include <string>
#include <cstdlib>
#include <variant>

static const std::string PROJECT_ROOT (std::getenv("SCRATCHQL_ROOT"));

using cell = std::variant<bool, long, double, std::string>;

#endif //DATABASE_CONFIG_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */