#ifndef DATABASE_CONTAINER_H
#define DATABASE_CONTAINER_H


namespace idx {
template <typename T>
struct Container {
    Container <T> * parent;
    Container <T> * smaller = nullptr;
    Container <T> * bigger  = nullptr;

    T val;

    Container (T val, Container * parent = nullptr) :
        val {val},
        parent {parent} {}
    ~Container() {
        delete smaller;
        delete bigger;
        if (parent->smaller == this)    parent->smaller = nullptr;
        else if (parent->bigger == this) parent->bigger = nullptr;
        else LOG (FATAL) << "Container went insane - parent lost its children!";
        delete parent;
    }
};
};

#endif //DATABASE_CONTAINER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */