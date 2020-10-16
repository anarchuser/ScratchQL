#define CATCH_CONFIG_RUNNER

#include "main.h"

int main (int argc, char * argv[]) {
    google::InitGoogleLogging(argv [0]);

    LOG (INFO) << "Start Testing";
    LOG (INFO) << "Project Folder: " << PROJECT_ROOT;
    LOG (INFO) << "DB Storage Loc: " << DB_DIR;

    for (auto const & folder : std::filesystem::directory_iterator (DB_DIR))
        std::filesystem::remove_all (folder);

    int result = Catch::Session().run (argc, argv);

    CLEAR();

    LOG (INFO) << "Stop Testing";

    return result;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */