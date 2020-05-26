#ifndef DATABASE_META_H
#define DATABASE_META_H

#include "../../Cell/Cell.h"

#include <variant>

namespace Key {
    using Reference = std::variant <std::monostate, std::string>;
}

struct Meta {
    std::string name;
    CellType dataType;
    KeyType keyType;
    Key::Reference reference;
    const int columnLength{};
    bool index{};
    bool nullable{};

    Meta() = default;
    Meta (std::string name, CellType dataType, KeyType keyType, bool index, bool nullable, int stringLength = 10);
    Meta (std::string name, CellType dataType, std::string reference, bool index, bool nullable, int stringLength = 10);

    bool operator == (Meta const & other) const;
};

std::ostream & operator << (std::ostream & os, Meta const & meta);


#endif //DATABASE_META_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */