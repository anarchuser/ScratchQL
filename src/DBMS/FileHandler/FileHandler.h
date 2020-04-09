#ifndef DATABASE_FILEHANDLER_H
#define DATABASE_FILEHANDLER_H

#include "../../config.h"

#include <cstdio>
#include <iostream>
#include <fstream>

struct FileHandler {
public:
    std::string path;

    explicit FileHandler (std::string path);

    void createLine (std::string const & content);                         // Appends a line
    std::string readLine (std::size_t index);                              //
    void updateLine (std::string const & content, std::size_t index);      // Writes a line
    void deleteLine (std::size_t index);                                   // Removes a line

    void deleteTable ();

private:
    void removePadding ();

};


#endif //DATABASE_FILEHANDLER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */