#ifndef DATABASE_MAIN_H
#define DATABASE_MAIN_H

/// Minimum LOG level. 0 = Everything, 1 = Ignore LOG (INFO), ...
#define GOOGLE_STRIP_LOG 0

#define KEEP_TEST_DBS false

#define CLEAR() \
    if (!KEEP_TEST_DBS) { \
        for (auto const & folder : std::filesystem::directory_iterator (DB_DIR)) \
            std::filesystem::remove_all (folder);                                \
    }; true

#include <glog/logging.h>
#include <catch2/catch.hpp>
#include <filesystem>

#include "../src/config.h"

#endif //DATABASE_MAIN_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */