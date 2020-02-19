#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "../../config.h"
#include "../Cell/Cell.h"
#include "../FileHandler/FileHandler.h"

#include <functional>
#include <unordered_map>
#include <variant>
#include <vector>

#include <iostream>

class Table {
private:
    FileHandler * file = nullptr;

    std::string database;
    std::string name;

    bool diskMode = false;

    std::vector <std::string> header;
    std::vector <KeyTypes> meta;
    std::unordered_map <std::string, std::vector <Cell>> table;
    std::vector <std::vector <std::reference_wrapper <Cell const>>> matrix;

    std::size_t rows = 0;
    std::size_t columns;

public:
    Table (std::vector <std::string> header, std::vector <KeyTypes> meta);
    ~Table ();

    void initDiskMode (std::string database, std::string name);

    void createRow (std::vector <Cell> const & row);
    void updateRow (std::size_t row_index, std::vector <Cell> const & row);
    std::unordered_map <std::string, Cell> readRow (std::size_t row_index) const;
    std::vector <Cell> readRowAsVector (std::size_t row_index) const;
    void deleteRow (std::size_t row_index);
