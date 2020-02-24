#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

#include <string>
#include <cstdlib>
#include <variant>
#include <glog/logging.h>

#include "DBMS/Cell/Cell.h"

static const std::string PROJECT_ROOT (std::getenv("SCRATCHQL_ROOT"));
static const std::string DATABASE_DIR = "/var/db/scratchql/";

enum KeyTypes {
    NORMAL,
    PRIMARY,
    UNIQUE,
    FOREIGN,
    INDEXED,
};

#endif //DATABASE_CONFIG_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */