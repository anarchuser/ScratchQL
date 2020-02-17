#include "Table.h"

Table::Table (std::vector <std::string> header, std::vector <KeyTypes> meta) :
    header {std::move (header)},
    meta {std::move (meta)},
    columns {this->header.size()}
{
    for (auto & key : header) {
        table.insert (std::make_pair (key, std::vector<Cell>()));
    }
}

void Table::removePadding () {
    for (std::size_t i = rows; i > 0; i--) {
        if (isRowEmpty (i - 1)) {
            for (const auto & key : header) {
                table [key].erase (table [key].begin() + i - 1);
            }
            rows--;
        }
    }
    rows = table [header [1]].size();
}

void Table::createRow (const std::vector <Cell> & row) {
    if (row.size() != columns) THROW (std::range_error ("Invalid amount of columns to insert"));

    for (int i = 0; i < row.size(); i++) {
        table [header [i]].push_back (row[i]);
    }
    rows++;
}
void Table::updateRow (std::size_t row_i, const std::vector <Cell> & row) {
    if (row.size() != columns) THROW (std::range_error ("Invalid amount of columns to update"));
    if (row_i >= rows) THROW (std::range_error ("Index for updateRow out of bounds"));

    for (std::size_t col_i = 0; col_i < row.size(); col_i++) {
        table [header [col_i]] [row_i] = row [col_i];
    }
}
std::unordered_map <std::string, Cell> Table::readRow (std::size_t index) const {
    if (index >= rows) THROW (std::range_error ("Index for readRow out of bounds"));

    std::unordered_map <std::string, Cell> row;
    int i = 0;
    for (const auto & key : header) {
        row.insert (std::make_pair (key, table.at(key) [index]));
    }
    return row;
}
std::vector <Cell> Table::readRowAsVector (std::size_t index) const {
    if (index >= rows) THROW (std::range_error ("Index for readRow out of bounds"));

   std::vector <Cell> row;
   for (const auto & key : header) {
       row.push_back (table.at(key) [index]);
   }
   return row;
}
void Table::deleteRow (std::size_t index) {
    if (index >= rows) THROW (std::range_error ("Index for deleteRow out of bounds"));

    for (const auto & key : header) {
        toNull (table [key] [index]);
    }
}

bool Table::isRowEmpty(std::size_t index) const {
    if (index >= rows) THROW (std::range_error ("Index for isRowEmpty out of bounds"));
    for (const auto & key : header) {
        if (!isCellEmpty (key, index)) return false;
    }
    return true;
}

bool Table::isCellEmpty (const std::string & key, std::size_t index) const {
    if (index >= rows) THROW (std::range_error ("Index for isCellEmpty out of bounds"));
    return !table.at (key) [index];
}

std::vector <Cell> & Table::operator [] (const std::string & key) {
    return table.at (key);
}
const std::vector <Cell> & Table::operator [] (const std::string & key) const {
    return table.at (key);
}
std::unordered_map <std::string, Cell> Table::operator [] (std::size_t index) {
    return readRow (index);
}

std::size_t Table::getRowCount() const {
    return rows;
}
std::size_t Table::getColumnCount() const {
    return columns;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */