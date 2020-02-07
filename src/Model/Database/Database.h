#ifndef DATABASE_DATABASE_H
#define DATABASE_DATABASE_H

#include "../../config.h"

#include <unordered_map>
#include <variant>
#include <vector>

class Database {
private:
    std::vector <std::string> header;
    std::unordered_map <std::string, std::vector <cell>> matrix;

public:
    Database (const std::vector <std::string> & header);

    void createRow (const std::vector <cell> & row);
    void updateRow (const std::vector <cell> & row);
    const std::unordered_map <std::string, cell> & readRow (std::size_t index) const;
    void deleteRow (std::size_t index);

          Database & operator [] (const std::string & key);
    const Database & operator [] (const std::string & key) const;
          Database & operator [] (std::size_t index);
    const Database & operator [] (std::size_t index) const;
};


#endif //DATABASE_DATABASE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */