#ifndef DATABASE_HELPER_H
#define DATABASE_HELPER_H

#include "../DBMS/Cell/Cell.h"

#include <iostream>
#include <vector>
#include <optional>
#include <sstream>
#include <unordered_map>

template <class K, class V>
std::ostream & operator << (std::ostream & os, std::unordered_map <K, V> const & map) {
    os << '{';
    std::vector <K> keys;
    for (auto const & entry : map)
        keys.push_back (entry.first);
    std::sort (keys.begin(), keys.end(), [](std::string a, std::string b) -> bool {return a < b;});

    bool first = true;
    for (auto const & key : keys) {
        if (!first) os << ", ";
        os << '"' << key << "\": " << map.at (key);
        first = false;
    }
    return os << '}';
}

template <class T>
std::ostream & operator << (std::ostream & os, std::vector <T> const & list) {
    os << '[';
    bool first = true;
    for (auto const & element : list) {
        if (!first) os << ", ";
        os << element;
        first = false;
    }
    return os << ']';
}
template <class T>
std::ostream & operator << (std::ostream & os, std::optional <T> maybe) {
    return maybe.has_value() ? os << * maybe : os << "nothing";
}

template <class T>
std::string operator + (T obj) {
    std::stringstream ss;
    ss << obj;
    return ss.str();
}

#endif //DATABASE_HELPER_H

/* Copyright (C) 2020 Aaron Alef */