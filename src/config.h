#ifndef DATABASE_CONFIG_H
#define DATABASE_CONFIG_H

/// Minimum LOG level. 0 = Everything, 1 = Ignore LOG (INFO), ...
#define GOOGLE_STRIP_LOG 0

#ifdef ERRORS_AS_WARNINGS
#define THROW(error) {              \
    LOG (WARNING) << error.what();  \
    throw error;                    \
}; true
#else
#define THROW(error) {              \
    LOG (ERROR) << error.what();    \
    throw error;                    \
}; true
#endif

#include <string>
#include <cstdlib>
#include <variant>
#include <filesystem>
#include <glog/logging.h>
#include <netinet/in.h>

#include "DBMS/Cell/Cell.h"
#include "Util/Helper.h"

#define ADDRESS "localhost"
#define PORT 32786

std::string const STR = std::string();

std::string const META_DIR (".meta");
std::string const INDEX_DIR (".index");
std::filesystem::path const DB_DIR ("/var/db/scratchql");
std::filesystem::path const PROJECT_ROOT = []() -> std::filesystem::path {
    char * envvar = std::getenv ("SCRATCHQL_ROOT");
    if (envvar) return envvar;
    throw (std::logic_error ("Couldn't find env var 'SCRATCHQL_ROOT'. Please set it to this project's folder."));
}();

enum KeyType {
    NORMAL,
    PRIMARY,
    FOREIGN,
};

std::string const VALID_QUERY_CHARS (" <>!=+-*/{}[]().,:;@#\"");



#endif //DATABASE_CONFIG_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */