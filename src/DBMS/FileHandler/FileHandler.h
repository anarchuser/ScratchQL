#ifndef DATABASE_FILEHANDLER_H
#define DATABASE_FILEHANDLER_H

#include "../../config.h"
#include "../Cell/Cell.h"
#include "../Table/Table.h"
#include "../Table/Meta/Meta.h"
#include "../../Util/Tokens.h"
#include "../../Language/Target/Target.h"

#include <cstdio>
#include <cctype>
#include <clocale>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


class FileHandler {
private:
    std::string db_root;
    std::string database;
    std::string name;
    std::string path;
    std::size_t lineLength;                                                       //for testing purposes until a meta file is created TODO: remove ASAP
    std::vector <std::size_t> columnLength;
    std::vector <CellType> columnType;

    [[nodiscard]] std::size_t checkLineLength(std::string const & content) const;
    std::vector <std::size_t> surplusColumnLengths(std::vector <Cell> const & contentVector);
    std::string fit (Cell const & cell, std::size_t length);

public:
    FileHandler(std::string const & database, std::string  table, std::vector <std::size_t> const & columnLen, std::vector <CellType>  colType);
    explicit FileHandler (Table const & table);

    void createLine (std::vector <Cell> const & content);                         // Appends a line
    [[nodiscard]] std::vector <Cell> readLine (std::size_t index) const;                                     //
    void updateLine (std::size_t index, std::vector <Cell> const & content);              // Writes a line
    void deleteLine (std::size_t index);                                   // Removes a line

    void clearLines () const;
    void deleteTable () const;
    void deleteDatabase () const;

    static void create (qy::Database const & db);
    static void create (qy::Table const & table);
    static void create (qy::Column const & column);
    static void create (qy::Row const & row);

    static void remove (qy::Database const & db);
    static void remove (qy::Table const & table);
    static void remove (qy::Column const & column);
    static void remove (qy::Row const & row);
};

std::size_t calcLineLength(std::vector <std::size_t> const & colLength);

#endif //DATABASE_FILEHANDLER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */