#ifndef DATABASE_FILEHANDLER_H
#define DATABASE_FILEHANDLER_H

#include "../../config.h"
#include "../Table/Meta/Meta.h"

#include <cstdio>
#include <cctype>
#include <clocale>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

struct FileHandler {
public:
    FileHandler(std::string const & database, std::string const & table, std::vector <int> const & columnLen, std::vector <CellType> const & colType);
    std::string db_root;
    std::string database;
    std::string name;
    std::string path;
    int lineLength;                                                       //for testing purposes until a meta file is created TODO: remove ASAP
    std::vector <int> columnLength;
    std::vector <CellType> columnType;

    explicit FileHandler (std::string path);

    void createLine (std::vector <Cell> const & content);                         // Appends a line
    std::vector <Cell> readLine (std::size_t index) const;                                     //
    void updateLine (std::size_t index, std::vector <Cell> const & content);              // Writes a line
    void deleteLine (std::size_t index);                                   // Removes a line

    static void cutTailingSpaces(std::string & content);
    static Cell writeToCell (std::string & inputString, CellType cellType);
    void clearLines () const;
    void deleteTable () const;
    void deleteDatabase () const;

private:
    void createDatabase();
    void createTable();
    static void cleanName(std::string & alnum_string);
    int checkLineLength(std::string const & content) const;
    std::vector <int> surplusColumnLengths(std::vector <Cell> const & contentVector);
};

int calcLineLength(std::vector <int> const & colLength);

#endif //DATABASE_FILEHANDLER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */