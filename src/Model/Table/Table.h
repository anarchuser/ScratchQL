#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H

#include "../../config.h"

#include <unordered_map>
#include <variant>
#include <vector>

class Table {
private:
    std::vector <std::string> header;
    std::vector <KeyTypes> meta;
    std::unordered_map <std::string, std::vector <cell>> table;

    void removePadding();

public:
    Table (std::vector <std::string> header, std::vector <KeyTypes> meta);

    void createRow (const std::vector <cell> & row);
    void updateRow (const std::vector <cell> & row);
    const std::unordered_map <std::string, cell> & readRow (std::size_t index) const;
    void deleteRow (std::size_t index);

          Table & operator [] (const std::string & key);
    const Table & operator [] (const std::string & key) const;
          Table & operator [] (std::size_t index);
    const Table & operator [] (std::size_t index) const;
};



#endif //DATABASE_TABLE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */