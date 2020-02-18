#ifndef DATABASE_FILEHANDLER_H
#define DATABASE_FILEHANDLER_H

#include "../../main.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

struct FileHandler {
public:
    std::string path;

    explicit FileHandler (std::string  path);

    void createLine (std::string const & content);                         // Appends a line
    std::string readLine (std::size_t ndex);                                     //
    void updateLine (std::string const & content, std::size_t index);      // Writes a line
    void deleteLine (std::size_t index);                                   // Removes a line

    void deleteTable ();

private:
    void createTable(std::string database, std::string name);
    void createDatabase(std::string name);
    void removePadding ();
};


#endif //DATABASE_FILEHANDLER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */