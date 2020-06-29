#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "../../config.h"
#include "../Cell/Cell.h"
#include "Index/Index.h"
#include "Meta/Meta.h"
#include "../../Util/Tokens.h"

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <variant>
#include <vector>
#include <ranges>

#include <iostream>

/// In-memory representation of one table; contains column names and cell data
class Table {
private:
    std::vector <std::string> header;
    std::vector <Meta> meta;
    std::unordered_map <std::string, std::vector <Cell>> table;

public:
    std::string const database;
    std::string const name;

    /// Creates a new Table where each element in `header` equals the name of one column
    explicit Table (std::vector <Meta> const & meta, std::string const & dbname, std::string const & tablename);

    /// Append row
    void createRow (std::vector <Cell> const & row);

    /// Replace row with index `row_index` with `row`
    void updateRow (std::size_t row_index, std::vector <Cell> const & row);

    /// Read row with index `row_index` as unordered hash map
    std::unordered_map <std::string, Cell> readRow (std::size_t row_index) const;

    /// Read row with index `row_index` as list of cells
    std::vector <Cell> readRowAsVector (std::size_t row_index) const;

    /// Remove row from table
    void deleteRow (std::size_t row_index);

    bool isRowEmpty (std::size_t row_index) const;
    static bool isRowEmpty (std::vector <Cell> const & row);
    bool isCellEmpty (std::string const & key, std::size_t row_index) const;

    std::vector <Cell> operator [] (std::string const & key) const;

    /** Returns row with index `row_index` as unordered hash map
     *
     * DO NOT USE TO RETRIEVE SINGLE CELLS!!!
     * If a single cell is needed, always use `table [col][row]`!
     * Never use table [row][col]!!! */
    std::unordered_map <std::string, Cell> operator [] (std::size_t row_index);

    std::vector <std::string> const & getHeader() const;

    std::vector <Meta> const & getMeta() const;
    std::vector <std::size_t> getColumnLengths() const;
    std::vector <CellType> getDataTypes() const;

    std::size_t rowCount() const;
    std::size_t columnCount() const;

    /// Returns true if table is empty
    bool operator ! () const;
};

std::ostream & operator << (std::ostream & os, Table const & table);


#endif //DATABASE_TABLE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */