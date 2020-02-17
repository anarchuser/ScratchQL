#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "../../config.h"
#include "../Cell/Cell.h"

#include <unordered_map>
#include <variant>
#include <vector>

#include <iostream>

class Table {
private:
    std::vector <std::string> header;
    std::vector <KeyTypes> meta;
    std::unordered_map <std::string, std::vector <Cell>> table;

    std::size_t rows = 0;
    std::size_t columns;


public:
    Table (std::vector <std::string> header, std::vector <KeyTypes> meta);

    void createRow (const std::vector <Cell> & row);
    void updateRow (std::size_t index, const std::vector <Cell> & row);
    std::unordered_map <std::string, Cell> readRow (std::size_t index) const;
    std::vector <Cell> readRowAsVector (std::size_t index) const;
    void deleteRow (std::size_t index);

    bool isRowEmpty (std::size_t index) const;
    bool isCellEmpty (const std::string & key, std::size_t index) const;

          std::vector <Cell> & operator [] (const std::string & key);
    const std::vector <Cell> & operator [] (const std::string & key) const;

    std::unordered_map <std::string, Cell> operator [] (std::size_t index);

    void removePadding();

    std::size_t getRowCount() const;
    std::size_t getColumnCount() const;
};



#endif //DATABASE_TABLE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */