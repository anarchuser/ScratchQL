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
    os  << "Name: \t'" << meta.name       << "'" << std::endl
        << "Type: \t"  << meta.dataType          << std::endl
        << "Key:  \t"  << meta.keyType           << std::endl
        << "Ref?: \t"  << (!meta.reference.index() ? "None" :
        std::get <std::string> (meta.reference)) << std::endl
        << "Index:\t" << meta.index              << std::endl
        << "Null?:\t" << meta.nullable           << std::endl;
    return os;
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

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */