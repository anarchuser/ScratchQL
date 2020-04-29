#ifndef DATABASE_INDEX_H
#define DATABASE_INDEX_H

#include "../../../config.h"
#include "Indices/interface.h"
#include "Indices/indices.h"

#include <memory>
#include <fstream>
#include <filesystem>

class Index final {
private:
    std::unique_ptr <IndexImpl> index;
    CellType dataType;
    bool unique;

public:
    Index (CellType dataType, bool isUnique) :
            dataType {dataType},
            unique {isUnique},
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

    Index (std::string const & database, std::string const & table, std::string const & column) {
        //TODO: Replace path by a proper one
        std::ifstream is;
        try {
            is = std::ifstream (STR+ "/var/log/ScratchQL/" + database + '/' + table + '/' + column + ".idx");
        } catch (std::exception & e) THROW (e);

        // TODO: Add path to error message
        if (!is.is_open()) THROW (std::ios_base::failure ("Can't open file"));
        int type; is >> type >> std::ws >> unique;
        * this = Index ((CellType) type, unique);
        // TODO: implement lodaing of null array
        // TODO: call index->load appropriately
        is.close();
    }

    Index (Index const & other) = delete;
    Index & operator = (Index && other) {
        this->index = std::move (index);
        return * this;
    }

    CellType getDataType() const { return dataType; };
    bool isUnique() const { return unique; };


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

    idx::Rows select (Cell const & cell) const {
        if (!cell) LOG (WARNING) << "Searching for cell of type 'void'...";
        else if (cell.index() != dataType) THROW (std::logic_error ("Invalid cell type to index!"));
        return index->select (cell);
    }

    std::string str() const { return index->str(); }
    std::ostream & operator << (std::ostream & os) {
        return os << dataType << ' ' << unique << '\n' << index->str();
    }

    void save (std::string const & database, std::string const & table, std::string const & column) const {
        std::ofstream os;
        try {
            os = std::ofstream (STR+ "/var/log/ScratchQL/" + database + '/' + table + '/' + column + ".idx", std::ios::trunc);
        } catch (std::exception & e) THROW (e);

        // TODO: Add path to error message
        if (!os.is_open()) THROW (std::ios_base::failure ("Can't open file"));
        os << dataType << ' ' << unique << '\n';
        os << index->dump() << std::endl;
        os.close();
    }
    std::string dump () const { return index->dump(); }
};


#endif //DATABASE_INDEX_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */