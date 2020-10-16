#include "Meta.h"

Meta::Meta (std::string name, CellType dataType, KeyType keyType, bool index, bool nullable, std::size_t stringLength) :
        name {std::move (name)},
        dataType {dataType},
        keyType {keyType},
        columnLength {dataType != CellType::TEXT ? CellLength[dataType] : stringLength},
        index {index},
        nullable {nullable}
{
    if (!dataType) THROW (std::range_error (STR+
    "A column can't have the type 'void' (As requested for '" + this->name + "')"));
    if (keyType == KeyType::FOREIGN) THROW (std::logic_error (STR+
    "Missing Reference of foreign key for column " + this->name));
}
Meta::Meta (std::string name, CellType dataType, std::string reference, bool index, bool nullable, std::size_t stringLength) :
        name {std::move (name)},
        dataType {dataType},
        keyType {FOREIGN},
        reference {std::move (reference)},
        columnLength {dataType != CellType::TEXT ? CellLength[dataType] : stringLength},
        index {index},
        nullable {nullable}
{
    if (!dataType) THROW (std::range_error (STR+
    "A column can't have the type 'void' (As requested for '" + this->name + "')"));
}

std::ostream & operator << (std::ostream & os, Meta const & meta) {
    return os << "N: " << meta.name
              << ",\tC: " << meta.dataType
              << ",\tK: " << meta.keyType
              << ",\tR: " << (!meta.reference.index() ? "None" :
                      std::get <std::string> (meta.reference))
              << ",\tI: " << meta.index
              << ",\t?: " << meta.nullable;
}

bool Meta::operator == (Meta const & other) const {
    return  name     == other.name     &&
            dataType == other.dataType &&
            keyType  == other.keyType  &&
            index    == other.index    &&
            nullable == other.nullable &&
            (!reference.index() ? true :
                    std::get <std::string> (reference) ==
                    std::get <std::string> (other.reference));
}

void Meta::save (std::vector <Meta> const & metae, const std::string & path){
    std::ofstream os;
    try {
        os = std::ofstream (path);
    } catch (std::exception & e) THROW (e);
    if (!os.is_open()) THROW (std::ios_base::failure (STR+ "Can't open file at '" + path + "'"));

    for (auto const & meta : metae) {
        os     << meta.dataType
        << ' ' << meta.keyType
        << ' ' << meta.index
        << ' ' << meta.nullable
        << ' ' << meta.name
        << ' ' << ((meta.reference.index()) ? std::get <std::string> (meta.reference) : "")
        << '\n';
    }
}
std::vector <Meta> Meta::load (const std::string & path) {

}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */