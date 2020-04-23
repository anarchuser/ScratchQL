#ifndef DATABASE_CONTAINER_H
#define DATABASE_CONTAINER_H

namespace idx {
template <typename T>
concept Streamable = requires (std::ostream & os, T const & t) { os << t; };

template <typename T>
//requires Streamable <T>
struct Container {
    Container <T> * parent;
    Container <T> * smaller = nullptr;
    Container <T> * bigger  = nullptr;

    T val;

    Container (T val, Container * parent = nullptr) :
        val {val},
        parent {parent} {}
    ~Container() {
        delete smaller; smaller = nullptr;
        delete bigger;  bigger  = nullptr;
        delete parent;
    }

    std::ostream & operator << (std::ostream & os) const {
//        if (smaller) os << * smaller;
//        os << val.first << "\t";
//        if (bigger) os << * bigger;
        return os << str();
    }
    std::string str () const {
        std::stringstream ss;
        if (smaller) ss << smaller->str();
        ss << val.first << "\t";
        if (bigger) ss << bigger->str();
        return ss.str();
    }
};
};

//template <idx::Streamable U, typename V>
//std::ostream & operator << (std::ostream & os, std::pair <U, V> const & val) {
//    return os << val.first;
//}

#endif //DATABASE_CONTAINER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */