#ifndef DATABASE_INDEX_H
#define DATABASE_INDEX_H

#include "../../../config.h"
#include "Indices/interface.h"
#include "Indices/indices.h"

class Index {
private:
    CellType dataType;
    bool isUnique;
    IndexImpl & index;

public:
    explicit Index (CellType dataType, bool isUnique) :
        dataType {dataType},
        isUnique {isUnique},
        index {[&]() -> IndexImpl & {
            switch (dataType) {
                case UNARY:
                    THROW (std::logic_error ("Column of type void not possible"));
                case SHORT:
                case LONG:
                    isUnique ? index = BTree() : index = TTree();
                    break;
                case BINARY:
                case TEXT:
                    index = HMap (isUnique && dataType == CellType::TEXT);
                    break;
                default:
                    LOG (FATAL) << "Cell type went insane = can't be larger than TEXT!";
            }
        }()} {}

    bool insert (Cell cell, std::size_t row) {
        return index.insert (cell, row);
    }

    bool remove (Cell cell, std::size_t row) {
        return index.remove (cell, row);
    }

    idx::Rows select (Cell const & cell) {
        return index.select (cell);
    }

    std::vector <idx::Rows> select_if (bool (check) (Cell const & cell)) {
        return std::move (index.select_if (check));
    }

    std::ostream & operator << (std::ostream & os) {
        return os << index.str();
    }
    std::string str() {
        return index.str();
    }
};


#endif //DATABASE_INDEX_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */