#ifndef DATABASE_HELPER_H
#define DATABASE_HELPER_H

#include <iostream>
#include <vector>

template <class T>
bool operator == (std::vector <T> const & one, std::vector <T> const & two) {
    if (one.size() != two.size()) return false;
    for (std::size_t idx = 0; idx < one.size(); idx++) {
        if (one [idx] != two [idx]) return false;
    }
    return true;
}
template <class T>
bool operator != (std::vector <T> const & one, std::vector <T> const & two) {
    return !(one == two);
}

//std::ostream & operator << (std::ostream & os, char const * string) {
//    return os << std::string (string);
//}
template <class T>
std::ostream & operator << (std::ostream & os, std::vector <T> list) {
    os << '[';
    bool first = true;
    for (auto const & element : list) {
        if (!first) os << ", ";
        os << element;
        first = false;
    }
    return os << ']';
}

#endif //DATABASE_HELPER_H

/* Copyright (C) 2020 Aaron Alef */