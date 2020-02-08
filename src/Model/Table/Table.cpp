#include "Table.h"

Table::Table (std::vector <std::string> header, std::vector <KeyTypes> meta) :
    header {std::move (header)},
    meta {std::move (meta)},
    columns {header.size()}
{
    for (auto & key : header) {
        table.insert (std::make_pair (key, std::vector<cell>()));
    }
}

void Table::createRow (const std::vector <cell> & row) {
    for (int i = 0; i < row.size(); i++) {
        table[header[i]].push_back (row[i]);
    }
    rows++;
}
void Table::updateRow (const std::vector <cell> & row) {}
const std::unordered_map <std::string, cell> & Table::readRow (std::size_t index) const {}
void Table::deleteRow (std::size_t index) {}

      Table & Table::operator [] (const std::string & key) {}
const Table & Table::operator [] (const std::string & key) const {}
      Table & Table::operator [] (std::size_t index) {}
const Table & Table::operator [] (std::size_t index) const {}

std::size_t Table::getRows() {
    return rows;
}
std::size_t Table::getColumns() {
    return columns;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */