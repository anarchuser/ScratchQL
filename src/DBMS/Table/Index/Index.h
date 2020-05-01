#ifndef DATABASE_INDEX_H
#define DATABASE_INDEX_H

#include "../../../config.h"
#include "../../../Util/Tokens.h"
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

    Index (std::string const & path) { load (path); }

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

    void save (std::string const & path) const {
        std::ofstream os;
        try {
            os = std::ofstream (path);
        } catch (std::exception & e) THROW (e);
        if (!os.is_open()) THROW (std::ios_base::failure (STR+ "Can't open file at '" + path + "'"));

        os << dataType << ' ' << unique << '\n';
        os << index->dump();
        os.close();
    }

    std::string dump () const { return index->dump(); }

private:
    void load (std::string const & path) {
        std::ifstream is;
        try {
            is = std::ifstream (path);
        } catch (std::exception & e) THROW (e);

        if (!is.is_open()) THROW (std::ios_base::failure (STR+ "Can't open file at '" + path + "'"));
        int type; is >> type >> std::ws >> unique;
        index = std::move (Index ((CellType) type, unique).index);

        {
            std::string line;
            std::getline (is, line);
            std::vector <std::size_t> nulls;
            for (auto const & str : sv::splitTokens (line, '\t')) nulls.push_back (std::stoi (str));
            index->load (nulls);
        }
        {
            std::vector <std::pair <Cell, std::vector <std::size_t>>> data;

            while (is) {
                std::string line;
                std::getline (is, line);
                std::vector <std::string> tokens = sv::splitTokens (line, '\t');
                if (tokens.empty()) continue;
                std::pair <Cell, std::vector <std::size_t>> row;
                // TODO: REPLACE `short(0)` BY AN ACTUAL STRING TO CELL CONVERSION!!!
                row.first = (dataType == CellType::TEXT) ? Cell (tokens.front()) : Cell (short(0));
                bool isFirst = true;
                for (auto const & val : tokens) {
                    if (isFirst) isFirst = false;
                    else try {
                        row.second.push_back (std::stoi (val));
                    } catch (std::exception & e) {
                        LOG (WARNING) << "Can't convert value '" << val << "' to int - ignoring it...";
                    };
                }
                data.push_back (row);
            }
            index->load (data);
        }
        is.close();
    }
};


#endif //DATABASE_INDEX_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */