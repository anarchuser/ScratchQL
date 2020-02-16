#include "Table.h"

Table::Table (std::vector <std::string> header, std::vector <KeyTypes> meta) :
    header {std::move (header)},
    meta {std::move (meta)},
    columns {header.size()}
{
    for (auto & key : header) {
        table.insert (std::make_pair (key, std::vector<Cell>()));
    }
}

void Table::removePadding () {
    for (int i = 0; i < rows; i++) {
        if (isRowEmpty (i)) {
            for (const auto & key : header) {
                table [key].erase (table [key].begin() + i);
            }
        }
    }
    rows = table [header [1]].size();
}

void Table::createRow (const std::vector <Cell> & row) {
    for (int i = 0; i < row.size(); i++) {
        table[header[i]].push_back (row[i]);
    }
    rows++;
}
void Table::updateRow (const std::vector <Cell> & row, std::size_t index) {
    if (index < 0 && index >= rows) throw std::range_error ("Index for updateRow out of bounds");
}
const std::unordered_map <std::string, Cell> & Table::readRow (std::size_t index) const {
    if (index < 0 && index >= rows) throw std::range_error ("Index for readRow out of bounds");
}
void Table::deleteRow (std::size_t index) {
    if (index < 0 && index >= rows) throw std::range_error ("Index for deleteRow out of bounds");
    for (const auto & key : header) {
        toNull (table [key] [index]);
    }
}

bool Table::isRowEmpty(std::size_t index) const {
    if (index < 0 && index >= rows) throw std::range_error ("Index for isRowEmpty out of bounds");
    for (const auto & key : header) {
//        if (!isCellEmpty (key, index)) return false;
    }
    return true;
}

bool Table::isCellEmpty (const std::string & key, std::size_t index) const {
    if (index < 0 && index >= rows) throw std::range_error ("Index for isCellEmpty out of bounds");
    return !table.at (key) [index];
}

      Table & Table::operator [] (const std::string & key) {}
const Table & Table::operator [] (const std::string & key) const {}
      Table & Table::operator [] (std::size_t index) {}
const Table & Table::operator [] (std::size_t index) const {}

std::size_t Table::getRowCount() const {
    return rows;
}
std::size_t Table::getColumnCount() const {
    return columns;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */