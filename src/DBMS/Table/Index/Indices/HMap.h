#ifndef DATABASE_HMAP_H
#define DATABASE_HMAP_H

#include "../../../../config.h"
#include "interface.h"

#include <unordered_map>
#include <iostream>

class HMap : public IndexImpl {
private:
    bool isUnique;
    std::unordered_map <Cell, std::vector <std::size_t>> map;
    std::vector <std::size_t> nulls;

public:
    HMap (bool isUnique) : isUnique {isUnique} {}

    bool insert (Cell cell, std::size_t row) {
        if (!cell) nulls.push_back (row);
        else {
            bool exists = select (cell).index();
            if (isUnique && exists) return false;
            if (exists) map.at (cell).push_back (row);
            else map.emplace (cell, std::vector <std::size_t> {row});
        }
        return true;
    }
    bool remove (Cell cell, std::size_t row) {
        if (!cell) return idx::eraseFromVector (nulls, row);
        if (!select (cell).index()) return false;
        if (isUnique) map.erase (cell);
        else return idx::eraseFromVector (map.at (cell), row);
        return true;
    }
    idx::Rows select (Cell const & cell) {
        if (!cell) return nulls;
        if (isUnique) try {
            return map.at (cell).front();
        } catch (std::out_of_range & e) {
            return idx::Rows();
        }
        return map.at (cell);
    }
    std::vector <idx::Rows> select_if (bool (check) (Cell const & cell)) {
        if (check (Cell())) return std::vector <idx::Rows> {nulls};
        std::vector <idx::Rows> selectedRows;
        for (auto const & cell : map) {
            if (check (cell.first)) {
                if (!isUnique) selectedRows.emplace_back (cell.second);
                else selectedRows.emplace_back (cell.second.front());
            }
        }
        return std::move (selectedRows);
    }

    std::ostream & operator << (std::ostream & os) {
        return os;
    }
    std::string str() {
        return std::string();
    }
};


#endif //DATABASE_HMAP_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */