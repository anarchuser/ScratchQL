#ifndef DATABASE_INDEX_H
#define DATABASE_INDEX_H

#include "../../../config.h"
#include "Indices/interface.h"
#include "Indices/indices.h"

#include <memory>


class Index {
private:
    CellType dataType;
    bool isUnique;
    std::unique_ptr <IndexImpl> index;

public:
    Index (CellType dataType, bool isUnique) :
        dataType {dataType},
        isUnique {isUnique},
        index {[&]() -> IndexImpl * {
            switch (dataType) {
                case UNARY:
                    THROW (std::logic_error ("Column of type void not possible"));
                case SHORT:
                case LONG:
                    if (isUnique) return new BTree();
                    return new TTree();
                case BINARY:
                case TEXT:
                    return new HMap (isUnique && dataType == CellType::TEXT);
                default:
                    LOG (FATAL) << "Cell type went insane = can't be larger than TEXT!";
            }
        }()} {}

    Index (Index const & other) = delete;
    Index & operator = (Index && other) {
        this->dataType = other.dataType;
        this->isUnique = other.isUnique;
        this->index = std::move (index);
        return * this;
    }

    bool insert (Cell cell, std::size_t row) {
        if (!cell) LOG (WARNING) << "Indexing cell of type 'void' (r " << row << ")...";
        else if (cell.index() != dataType) THROW (std::logic_error ("Invalid cell type to index!"));
        return index->insert (cell, row);
    }

    bool remove (Cell cell, std::size_t row) {
        if (!cell) LOG (WARNING) << "Removing cell of type 'void' (r " << row << ")...";
        else if (cell.index() != dataType) THROW (std::logic_error ("Invalid cell type to index!"));
        return index->remove (cell, row);
    }

    idx::Rows select (Cell const & cell) {
        if (!cell) LOG (WARNING) << "Searching for cell of type 'void'...";
        else if (cell.index() != dataType) THROW (std::logic_error ("Invalid cell type to index!"));
        return index->select (cell);
    }

    std::string str() const {
        return index->str();
    }
    std::istream & operator >> (std::istream & is) {
        int type; is >> type >> std::ws >> isUnique;
        * this = Index ((CellType) type, isUnique);
        // TODO: call index->load appropriately
        return is;
    }
    std::ostream & operator << (std::ostream & os) {
        return os << dataType << ' ' << isUnique << '\n' << index->str();
    }
};


#endif //DATABASE_INDEX_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */