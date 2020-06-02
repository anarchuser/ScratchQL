#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "../../config.h"
#include "../Cell/Cell.h"
#include "Index/Index.h"
#include "../FileHandler/FileHandler.h"
#include "Meta/Meta.h"

#include <functional>
#include <unordered_map>
#include <variant>
#include <vector>

#include <iostream>

/// In-memory representation of one table; contains column names and cell data
class Table {
private:
    std::vector <std::string> header;
    std::unordered_map <std::string, Meta const> meta;
    std::unordered_map <std::string, std::vector <Cell>> table;
    std::vector <std::vector <Cell const *>> matrix;
    FileHandler tablefile;

    std::string database, name;
    FileHandler * file = nullptr;
    bool diskMode = false;

    std::size_t row_count = 0;
    std::size_t col_count = 0;

public:
    /// Creates a new Table where each element in `header` equals the name of one column
    // TODO: Create Constructor taking metadata which actually creates a new table
    explicit Table (std::vector <Meta> const & meta, std::string const & dbname, std::string const & tablename);

    ~Table();

    /// Initialises syncing with file on disk
    void initDiskMode (std::string database, std::string table);

    /// Append `row` to this table. Throws if its size doesn't match the amount of columns
    // TODO: Call FileHandler::createLine accordingly
    void createRow (std::vector <Cell> const & row);

    /// Replace row with index `row_index` with `row`
    // TODO: Call FileHandler::updateLine accordingly
    void updateRow (std::size_t row_index, std::vector <Cell> const & row, bool updateFileHandler = true);

    /// Read row with index `row_index` as unordered hash map
    std::unordered_map <std::string, Cell> readRow (std::size_t row_index) const;
    std::unordered_map <std::string, Cell> readRow (std::size_t row_index);

    /// Read row with index `row_index` as list of cells
    // TODO: Call FileHandler::readLine accordingly
    std::vector <Cell> readRowAsVector (std::size_t row_index) const;
    std::vector <Cell> readRowAsVector (std::size_t row_index);

    void deleteTable ();

    /// Delete row with index `row_index`
    // TODO: Call FileHandler::deleteLine accordingly
    void deleteRow (std::size_t row_index);

    bool isRowEmpty (std::size_t row_index) const;
    bool isCellEmpty (std::string const & key, std::size_t row_index) const;

    std::vector <Cell>       & operator [] (std::string const & key);
    std::vector <Cell> const & operator [] (std::string const & key) const;

    /** Returns row with index `row_index` as unordered hash map
     *
     * DO NOT USE TO RETRIEVE SINGLE CELLS!!!
     * If a single cell is needed, always use `table [col][row]`!
     * Never use table [row][col]!!! */
    std::unordered_map <std::string, Cell> operator [] (std::size_t row_index);

    /// Removes any row where each element is NULL
    // TODO: Check if this is really needed
    // TODO: Call FileHandler::removePadding accordingly
    void removePadding() ;

    std::vector <std::string> const & getHeader() const;

    std::unordered_map <std::string, Meta const> const & getMeta() const;
    std::vector <Meta> getMetaAsVector() const;
    std::vector <std::size_t> getMetaColLength(std::vector <Meta> const & meta);
    std::vector <CellType> getMetaDataType(std::vector <Meta> const & meta);

    /// Returns a reference of the current table. Do not use concurrently.
    std::vector <std::vector <Cell const *>> const & getContent() const;
    std::size_t getRowCount() const;
    std::size_t getColumnCount() const;

    /// Returns true if table is empty. Removes empty rows.
    bool operator ! ();
    /// Returns true if table is empty
    bool operator ! () const;
};

std::ostream & operator << (std::ostream & os, Table const & table);


#endif //DATABASE_TABLE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */