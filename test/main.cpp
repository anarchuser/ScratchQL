#define CATCH_CONFIG_RUNNER

#include "main.h"

int main (int argc, char * argv[]) {
    google::InitGoogleLogging(argv [0]);

    LOG (INFO) << "Start Testing";
    LOG (INFO) << "Project Folder: " << PROJECT_ROOT;
    LOG (INFO) << "DB Storage Loc: " << DB_DIR;

    std::cout << "Project Folder: " << PROJECT_ROOT << std::endl;
    std::cout << "Database Storage Location: " << DB_DIR << std::endl << std::endl;

    int result = Catch::Session().run (argc, argv);

    LOG (INFO) << "Stop Testing";

    return result;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */