#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "../../config.h"
#include "../Cell/Cell.h"

#include <functional>
#include <unordered_map>
#include <variant>
#include <vector>

#include <iostream>

class Table {
private:
    std::vector <std::string> header;
    std::vector <KeyTypes> meta;
    std::unordered_map <std::string, std::vector <Cell>> table;
    std::vector <std::vector <std::reference_wrapper <Cell const>>> matrix;

    std::size_t rows = 0;
    std::size_t columns;

public:
    Table (std::vector <std::string> header, std::vector <KeyTypes> meta);

    void createRow (std::vector <Cell> const & row_index);
    void updateRow (std::size_t row_index, std::vector <Cell> const & row);
    std::unordered_map <std::string, Cell> readRow (std::size_t row_index) const;
    std::vector <Cell> readRowAsVector (std::size_t row_index) const;
    void deleteRow (std::size_t row_index);

    bool isRowEmpty (std::size_t row_index) const;
    bool isCellEmpty (std::string const & key, std::size_t row_index) const;

    std::vector <Cell>       & operator [] (std::string const & key);
    std::vector <Cell> const & operator [] (std::string const & key) const;

    std::unordered_map <std::string, Cell> operator [] (std::size_t row_index);

    void removePadding();

    std::size_t getRowCount() const;
    std::size_t getColumnCount() const;
};



#endif //DATABASE_TABLE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */