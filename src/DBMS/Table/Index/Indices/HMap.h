#ifndef DATABASE_HMAP_H
#define DATABASE_HMAP_H

#include "../../../../config.h"
#include "interface.h"

#include <unordered_map>

class HMap : public IndexImpl {
private:
    bool isUnique;
    std::unordered_map <Cell, std::vector <std::size_t>> map;

public:
    HMap (bool isUnique) : isUnique {isUnique} {}

    bool insert (Cell cell, std::size_t row) {
        bool exists = select (cell).index();
        if (isUnique && exists) return false;
        if (exists) map.at (cell).push_back (row);
        else map.emplace (cell, std::vector <std::size_t> {row});
        return true;
    }
    bool remove (Cell cell, std::size_t row) {
        if (!select (cell).index()) return false;
        if (isUnique) map.erase (cell);
        else {
            auto & rows = map.at (cell);
            for (auto i = rows.begin(), last = rows.end(); i != last;) {
                if (* i == row) rows.erase (i);
                else ++i;
            }
        }
        return false;
    }
    idx::Rows select (Cell const & cell) {
        if (isUnique) try {
            return map.at (cell).front();
        } catch (std::out_of_range & e) {
            return idx::Rows();
        }
        return map.at (cell);
    }
    std::vector <idx::Rows> const & select_if (bool (check) (Cell const & cell)) {
        std::vector <idx::Rows> selectedRows;
        for (auto const & cell : map) {
            if (check (cell.first)) {
                if (!isUnique) selectedRows.emplace_back (cell.second);
                else selectedRows.emplace_back (cell.second.front());
            }
        }
        return std::move (selectedRows);
    }
};


#endif //DATABASE_HMAP_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */