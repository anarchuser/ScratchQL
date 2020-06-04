#ifndef DATABASE_INDEX_H
#define DATABASE_INDEX_H

#include "../../../config.h"
#include "../../../Util/Tokens.h"
#include "../../FileHandler/FileHandler.h"
#include "Indices/interface.h"
#include "Indices/indices.h"

#include <memory>
#include <fstream>
#include <experimental/filesystem>

class Index final {
private:
    std::unique_ptr <IndexImpl> index;
    CellType dataType;
    bool unique;

public:
    Index (CellType dataType, bool isUnique);
    explicit Index (std::string const & path);

    Index (Index const & other) = delete;
    Index & operator = (Index && other);

    [[nodiscard]] CellType getDataType() const;
    [[nodiscard]] bool isUnique() const;

    bool insert (Cell cell, std::size_t row);
    bool remove (Cell cell, std::size_t row);
    [[nodiscard]] idx::Rows select (Cell const & cell) const;

    [[nodiscard]] std::string str() const;
    [[nodiscard]] std::ostream & operator << (std::ostream & os) const;

    void save (std::string const & path) const;

    [[nodiscard]] std::string dump() const;

private:
    void load (std::string const & path);

};


#endif //DATABASE_INDEX_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */