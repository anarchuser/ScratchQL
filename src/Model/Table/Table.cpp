#include "Table.h"

Table::Table (std::vector <std::string> header, std::vector <KeyTypes> meta) :
    header {std::move (header)},
    meta {std::move (meta)},
    columns {this->header.size()}
{
    for (auto const & key : header) {
        table.insert (std::make_pair (key, std::vector<Cell>()));
    }
}

void Table::removePadding () {
    for (std::size_t i = rows; i > 0; i--) {
        if (isRowEmpty (i - 1)) {
            for (auto const & key : header) {
                table [key].erase (table [key].begin() + i - 1);
            }
            rows--;
        }
    }
    rows = table [header [1]].size();
}

void Table::createRow (std::vector <Cell> const & row) {
    if (row.size() != columns) throw (std::range_error ("Invalid amount of columns to insert"));

    matrix.emplace_back (std::vector <std::reference_wrapper <Cell const>> ());
    for (int col_index = 0; col_index < row.size(); col_index++) {
        table [header [col_index]].push_back (row [col_index]);
        matrix [rows].push_back (std::reference_wrapper <Cell const> (table [header [col_index]] [rows]));
    }
    rows++;
}
void Table::updateRow (std::size_t row_index, std::vector <Cell> const & row) {
    if (row.size() != columns) throw (std::range_error ("Invalid amount of columns to update"));
    if (row_index >= rows) throw (std::range_error ("Index for updateRow out of bounds"));

    for (std::size_t col_index = 0; col_index < row.size(); col_index++) {
        table [header [col_index]] [row_index] = row [col_index];
        matrix [col_index] [row_index] = std::reference_wrapper <Cell> (table [header [col_index]] [row_index]);
    }
}
std::unordered_map <std::string, Cell> Table::readRow (std::size_t row_index) const {
    if (row_index >= rows) throw (std::range_error ("Index for readRow out of bounds"));

    std::unordered_map <std::string, Cell> row;
    for (auto const & key : header) {
        row.insert (std::make_pair (key, table.at (key) [row_index]));
    }
    return row;
}
std::vector <Cell> Table::readRowAsVector (std::size_t row_index) const {
    if (row_index >= rows) throw (std::range_error ("Index for readRow out of bounds"));

   std::vector <Cell> row;
   for (auto const & key : header) {
       row.push_back (table.at (key) [row_index]);
   }
   return row;
}
void Table::deleteRow (std::size_t row_index) {
    if (row_index >= rows) throw (std::range_error ("Index for deleteRow out of bounds"));

    for (auto const & key : header) {
        toNull (table [key] [row_index]);
    }
}

bool Table::isRowEmpty(std::size_t row_index) const {
    if (row_index >= rows) throw (std::range_error ("Index for isRowEmpty out of bounds"));

    for (const auto & key : header) {
        if (!isCellEmpty (key, row_index)) return false;
    }
    return true;
}

bool Table::isCellEmpty (std::string const & key, std::size_t row_index) const {
    if (row_index >= rows) throw (std::range_error ("Index for isCellEmpty out of bounds"));
    return !table.at (key) [row_index];
}

std::vector <Cell> & Table::operator [] (std::string const & key) {
    return table.at (key);
}
const std::vector <Cell> & Table::operator [] (std::string const & key) const {
    return table.at (key);
}
std::unordered_map <std::string, Cell> Table::operator [] (std::size_t row_index) {
    return readRow (row_index);
}

std::size_t Table::getRowCount() const {
    return rows;
}
std::size_t Table::getColumnCount() const {
    return columns;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */