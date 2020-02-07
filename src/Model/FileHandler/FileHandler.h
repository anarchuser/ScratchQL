#ifndef DATABASE_FILEHANDLER_H
#define DATABASE_FILEHANDLER_H

#include "../../main.h"

#include <cstdio>
#include <iostream>
#include <fstream>

struct FileHandler {
public:
    std::string path;

    explicit FileHandler (std::string  path);

    void createLine (const std::string & content);                  // Appends a line
    std::string readLine (long index);                              //
    void updateLine (const std::string & content, long index);      // Writes a line
    void deleteLine (long index);                                   // Removes a line

    void deleteTable ();

private:
    void removePadding ();

};


#endif //DATABASE_FILEHANDLER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */