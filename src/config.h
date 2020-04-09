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
#include <glog/logging.h>
#include <netinet/in.h>

#include "DBMS/Cell/Cell.h"

#define IP_ADDRESS_FAMILY AF_INET
#define ADDRESS "localhost"
#define PORT 32786

static std::string const DATABASE_DIR = "/var/db/scratchql/";
static std::string const PROJECT_ROOT = []()->std::string const {
    char * envvar = std::getenv ("SCRATCHQL_ROOT");
    if (envvar) return std::string (envvar);
    throw (std::logic_error ("Couldn't find env var 'SCRATCHQL_ROOT'. Please set it to this project's folder."));
}();

enum KeyTypes {
    NORMAL,
    PRIMARY,
    UNIQUE,
    FOREIGN,
    INDEXED,
};

static std::string const VALID_QUERY_CHARS (" <>!=+-*/{}[]().,:;@#\"");

static auto const STR = std::string();


#endif //DATABASE_CONFIG_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */