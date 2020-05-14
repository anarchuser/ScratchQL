#include "Table.h"

Table::Table (std::vector <Meta> const & meta, std::string const & dbname, std::string const & tablename):
    tablefile{FileHandler(dbname, tablename, getMetaColLength(meta), getMetaDataType(meta))}
{
    for (auto const & column : meta) {
        if (!column.name.empty()) {
            this->header.emplace_back (column.name);
            this->meta.insert (std::make_pair (column.name, column));
            col_count++;
        }
    }
    if (col_count < 1) THROW (std::range_error ("The list of column names is empty"));
    for (auto const & key : header) {
        table.insert (std::make_pair (key, std::vector<Cell>()));
    }
}

Table::~Table () {
    if (diskMode) delete file;
}

void Table::initDiskMode (std::string database, std::string table) {
    database = std::move (database);
    name = std::move (table);

    //file = new FileHandler ("database, table");               //TODO: implement or delete

    diskMode = true;
}

void Table::removePadding () {
    LOG (INFO) << "Removing Padding of Table...";
    LOG_ASSERT (row_count <= table.at (header [0]).size());

    for (std::size_t i = table.at (header [0]).size(); i > 0; i--) {
        if (isRowEmpty (i - 1)) {
            for (auto const & key : header) {
                table [key].erase (table [key].begin() + i - 1);
            }
        }
    }

//    file->removePadding ();

    LOG_ASSERT (row_count == table.at (header [0]).size());
    tablefile.clearLines();
    LOG (INFO) << "Removed Padding of Table.";
}

void Table::createRow (std::vector <Cell> const & row) {
    LOG (INFO) << "Creating Row in Table...";

    if (row.size() != col_count) {
        THROW (std::range_error ("Invalid amount of columns to insert"));
    }

    matrix.emplace_back (std::vector <Cell const *>());
    for (int col_index = 0; col_index < row.size(); col_index++) {
        table [header [col_index]].push_back (row [col_index]);
        matrix [getRowCount()].push_back (& table [header [col_index]] [getRowCount()]);
    }
    row_count++;

    tablefile.createLine(row);
    LOG (INFO) << "Created Row in Table.";
}
void Table::updateRow (std::size_t row_index, std::vector <Cell> const & row, bool updateFileHandler) {
    LOG (INFO) << "Updating Row in Table...";

    if (row.size() != col_count) {
        THROW (std::range_error ("Invalid amount of columns to update"));
    }
    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for updateRow out of bounds"));
    }

    for (std::size_t col_index = 0; col_index < row.size(); col_index++) {
        table [header [col_index]] [row_index] = row [col_index];
        matrix [col_index] [row_index] = & table [header [col_index]] [row_index];
    }

    if (updateFileHandler) {
        tablefile.updateLine(row_index, row);
    }
    LOG (INFO) << "Updated Row in Table.";
}
std::unordered_map <std::string, Cell> Table::readRow (std::size_t row_index) const {
    LOG (INFO) << "Reading Row from Table...";

    std::vector <Cell> row_list = readRowAsVector (row_index);
    std::unordered_map <std::string, Cell> row_map;
    for (std::size_t idx = 0; idx < header.size(); idx++) {
        row_map.insert ({header [idx], row_list [idx]});
    }

    LOG (INFO) << "Read Row from Table.";

    return row_map;
}
std::unordered_map <std::string, Cell> Table::readRow (std::size_t row_index) {
    readRowAsVector (row_index);
    auto const ref = this;
    return ref->readRow (row_index);
}

//reads from file and updates table in memory (mind non-const)
std::vector <Cell> Table::readRowAsVector (std::size_t row_index) {
    LOG (INFO) << "Reading Row as Vector from Table...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for readRow out of bounds"));
    }

    std::vector <Cell> row = tablefile.readLine(row_index);
    std::stringstream tmp_line;
    updateRow(row_index, row, false);

// once Flags (like "changed) are implemented, this can be used instead of storage access
//   for (auto const & key : header) {
//       row.push_back (table.at (key) [row_index]);
//   }
   LOG (INFO) << "Read Row as Vector from Table.";
   return std::move (row);
}

// reads data from table in memory
std::vector <Cell> Table::readRowAsVector (std::size_t row_index) const{
    LOG (INFO) << "Reading Row as Vector from Table...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for readRow out of bounds"));
    }
    std::vector <Cell> row;

   for (auto const & key : header) {
       row.push_back (table.at (key) [row_index]);
   }
    LOG (INFO) << "Read Row as Vector from Table.";
    return std::move (row);
}
void Table::deleteRow (std::size_t row_index) {
    LOG (INFO) << "Deleting Row from Table...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for deleteRow out of bounds"));
    }

    for (auto const & key : header) {
        toNull (table [key] [row_index]);
    }
    row_count--;
    tablefile.deleteLine(row_index);

    LOG (INFO) << "Deleted Row from Table.";
}

bool Table::isRowEmpty(std::size_t row_index) const {
    LOG (INFO) << "Checking if Row in Table is empty...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for isRowEmpty out of bounds"));
    }

    for (const auto & key : header) {
        if (!isCellEmpty (key, row_index)) return false;
    }

    LOG (INFO) << "Checked Row from Table for emptiness.";

    return true;
}

bool Table::isCellEmpty (std::string const & key, std::size_t row_index) const {
    LOG (INFO) << "Checking if Cell is empty...";

    if (row_index >= table.at (header [0]).size()) {
        THROW (std::range_error ("Index for isCellEmpty out of bounds"));
    }
    bool isEmpty = !table.at (key) [row_index];

    LOG (INFO) << "Checked Cell for emptiness.";

    return isEmpty;
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

std::vector <std::string> const & Table::getHeader() const {
    return header;
}
std::unordered_map <std::string, Meta const> const & Table::getMeta() const {
    return meta;
}
std::vector <Meta> Table::getMetaAsVector() const {
    std::vector <Meta> meta_v;
    for (const auto & col : header) meta_v.push_back (meta.at (col));
    return std::move (meta_v);
}
std::vector <int> const Table::getMetaColLength(std::vector <Meta> const & meta){
    std::vector <int> columnLengths;
    for (auto const & counter : meta){
        columnLengths.emplace_back(counter.columnLength);
    }
    return std::move(columnLengths);
}
std::vector <CellType> const Table::getMetaDataType(std::vector <Meta> const & meta){
    std::vector <CellType> columnTypes;
    for (auto const & counter : meta){
        columnTypes.emplace_back(counter.dataType);
    }
    return std::move(columnTypes);
}
std::vector <std::vector <Cell const *>> const & Table::getContent() const {
    return matrix;
}
std::size_t Table::getRowCount() const {
    LOG_ASSERT (row_count <= table.at (header [0]).size());
    return row_count;
}
std::size_t Table::getColumnCount() const {
    LOG_ASSERT (col_count > 0 && col_count == header.size());
    return col_count;
}

bool Table::operator ! () {
    removePadding();
    return !getRowCount();
}

bool Table::operator ! () const {
    return !getRowCount();
}

std::ostream & operator << (std::ostream & os, Table const & table) {
    if (table.getColumnCount()) {
        for (auto const & col : table.getHeader()) os << col << "\t";
        os << std::endl;
        if (table.getRowCount ()) {
            os << std::endl;
            for (size_t row = 0; row < table.getRowCount (); row++) {
                for (auto const & cell : table.readRowAsVector (row)) os << cell << "\t";
                os << std::endl;
            }
        }
    }
    return os;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */