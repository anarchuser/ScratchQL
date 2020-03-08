#ifndef DATABASE_FILEHANDLER_H
#define DATABASE_FILEHANDLER_H

#include "../../main.h"

#include <cstdio>
#include <cctype>
#include <clocale>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

struct FileHandler {
public:
    FileHandler(std::string & database, std::string & table);
    std::string db_root;
    std::string & database;
    std::string & name;
    std::string path;
    int tmp_line_length;                                                   //for testing purposes until a meta file is created TODO: remove ASAP

    void createLine (std::string const & content);                         // Appends a line
    std::string readLine (std::size_t index);                                     //
    void updateLine (std::size_t index, std::string content);              // Writes a line
    void deleteLine (std::size_t index);                                   // Removes a line

    void cutTailingSpaces(std::string & content);
    void clearLines ();
    void deleteTable ();
    void deleteDatabase ();

private:
    void createDatabase();
    void createTable();
    void cleanName(std::string & alnum_string);
    int checkLineLength(std::string const & content);
};


#endif //DATABASE_FILEHANDLER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */