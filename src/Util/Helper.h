#ifndef DATABASE_HELPER_H
#define DATABASE_HELPER_H

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

#endif //DATABASE_HELPER_H

/* Copyright (C) 2020 Aaron Alef */