#ifndef DATABASE_CELL_H
#define DATABASE_CELL_H

#include <stdexcept>
#include <string>
#include <variant>

#include <iostream>

using Cell = std::variant<bool, long, double, std::string>;

bool operator ! (const Cell &);
std::ostream & operator << (std::ostream &, const Cell &);

void toNull (Cell &);

class Visitor {
    std::ostream & os;

public:
    explicit Visitor (std::ostream & os) : os {os} {}

    void operator () (bool);
    void operator () (long);
    void operator () (double);
    void operator () (const std::string &);
};

#endif //DATABASE_CELL_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */