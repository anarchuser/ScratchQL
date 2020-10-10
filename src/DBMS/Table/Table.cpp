#include "Table.h"


Table::Table (std::vector <Meta> const & meta, std::string dbname, std::string tablename):
    database {std::move(dbname)},
    name {std::move(tablename)}
{
    if (meta.empty()) THROW (std::invalid_argument ("Expected some columns; found none!"));
    for (auto const & column : meta) {
        sv::checkName (column.name);
        this->header.emplace_back (column.name);
        this->meta.push_back (column);
    }
    for (auto const & key : header) {
        table.insert (std::make_pair (key, std::vector<Cell>()));
    }
}

Table::Table (qy::Table const & table):
    Table (* table.metae, table.parent.name, table.name) {}

void Table::createRow (std::vector <Cell> const & row) {
    LOG (INFO) << "Creating Row in Table...";

    if (row.size() != columnCount()) {
        THROW (std::range_error ("Invalid amount of columns to insert"));
    }

    for (std::size_t col_index = 0; col_index < row.size(); col_index++) {
        std::vector <Cell> & column = table [header [col_index]];
        column.push_back (row [col_index]);
    }

    LOG (INFO) << "Created Row in Table.";
}
void Table::updateRow (std::size_t row_index, std::vector <Cell> const & row) {
    LOG (INFO) << "Updating Row in Table...";

    if (row.size() != columnCount ()) {
        THROW (std::range_error ("Invalid amount of columns to update"));
    }
    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for updateRow out of bounds"));
    }

    for (std::size_t col_index = 0; col_index < row.size(); col_index++) {
        table [header [col_index]] [row_index] = row [col_index];
    }

    LOG (INFO) << "Updated Row in Table.";
}
std::unordered_map <std::string, Cell> Table::readRow (std::size_t row_index) const {
    LOG (INFO) << "Reading Row from Table...";

    std::unordered_map <std::string, Cell> row_map;
    for (auto const & key : header) {
        row_map.insert ({key, table.at (key) [row_index]});
    }

    LOG (INFO) << "Read Row from Table.";

    return row_map;
}

// reads data from table in memory
std::vector <Cell> Table::readRowAsVector (std::size_t row_index) const{
    LOG (INFO) << "Reading Row as Vector from Table...";

    if (row_index >= rowCount()) {
        THROW (std::range_error ("Index for readRow out of bounds"));
    }

    std::vector <Cell> row;
    for (auto const & key : header) {
        row.emplace_back (table.at (key) [row_index]);
    }

    LOG (INFO) << "Read Row as Vector from Table.";
    return row;
}
void Table::deleteRow (std::size_t row_index) {
    LOG (INFO) << "Deleting Row from Table...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for deleteRow out of bounds"));
    }

    for (auto const & key : header) {
        table [key].erase (table [key].begin() + row_index);
    }

    LOG (INFO) << "Deleted Row from Table.";
}

bool Table::isRowEmpty (std::size_t row_index) const {
    LOG (INFO) << "Checking if Row in Table is empty...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for isRowEmpty out of bounds"));
    }

    for (const auto & key : header) if (!isCellEmpty (key, row_index)) return false;
    return true;
}
bool Table::isRowEmpty (std::vector <Cell> const & row) {
    LOG (INFO) << "Checking if Row is empty...";

    for (auto const & cell : row) if (!cell) return false;
    return true;
}

bool Table::isCellEmpty (std::string const & key, std::size_t row_index) const {
    LOG (INFO) << "Checking if Cell is empty...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for isCellEmpty out of bounds"));
    }
    return !table.at (key) [row_index];
}

std::vector <Cell> Table::operator [] (std::string const & key) const {
    return table.at (key);
}
std::unordered_map <std::string, Cell> Table::operator [] (std::size_t row_index) {
    return readRow (row_index);
}

std::vector <std::string> const & Table::getHeader() const {
    return header;
}
std::vector <Meta> const & Table::getMeta() const {
    return meta;
}
std::vector <std::size_t> Table::getColumnLengths () const {
    std::vector <std::size_t> lengths;
    for (auto const & element : meta) lengths.push_back (element.columnLength);
    return lengths;
}
std::vector <CellType> Table::getDataTypes () const {
    std::vector <CellType> types;
    for (auto const & element : meta) types.push_back (element.dataType);
    return types;
}
std::size_t Table::rowCount() const {
    return table.at (header.front()).size();
}
std::size_t Table::columnCount() const {
    return header.size();
}

bool Table::operator ! () const {
    return ! rowCount ();
}

std::ostream & operator << (std::ostream & os, Table const & table) {
    if (table.columnCount ()) {
        for (auto const & col : table.getHeader()) os << col << "\t";
        os << std::endl;
        if (table.rowCount ()) {
            os << std::endl;
            for (size_t row = 0; row < table.rowCount (); row++) {
                for (auto const & cell : table.readRowAsVector (row)) os << cell << "\t";
                os << std::endl;
            }
        }
    }
    return os;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */
