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
            if (isUnique && !map [cell].empty()) {
                LOG (WARNING) << "Cell already exists in Hash Map with unique indices";
                return false;
            }
            map [cell].push_back (row);
        }
        return true;
    }
    bool remove (Cell cell, std::size_t row) {
        if (!cell) return std::erase (nulls, row);
        if (!select (cell).index()) {
            LOG (WARNING) << "Can't remove cell from Hash Map as it can't be found";
            return false;
        }
        if (isUnique) map.erase (cell);
        else return std::erase (map.at (cell), row);
        return true;
    }
    idx::Rows select (Cell const & cell) const {
        if (!cell) return nulls;
        try {
            if (map.at (cell).empty ()) return std::monostate ();
            if (isUnique) return map.at (cell).front ();
            return map.at (cell);
        } catch (std::exception & e) {
            (LOG (WARNING) << "Can't find given value (" < cell) << ") in Hash Map";
            return std::monostate();
        }
    }

    std::string str() const {
        return std::string();
    }
};


#endif //DATABASE_HMAP_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */