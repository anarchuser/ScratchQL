#ifndef DATABASE_TTREE_H
#define DATABASE_TTREE_H

#include "../../../../config.h"
#include "interface.h"

class TTree : public IndexImpl {
private:
    Cell c;
    std::vector <std::size_t> rs;
    TTree * smaller = nullptr;
    TTree * bigger  = nullptr;
    std::vector <std::size_t> nulls;

    void append_if (std::vector <idx::Rows> & v, bool (check) (Cell const & cell)) {
        if (check (c)) v.push_back (rs);
        if (smaller) smaller->append_if (v, check);
        if (bigger) bigger->append_if (v, check);
    }

public:
    TTree () = default;
    TTree (Cell cell, std::size_t row) : c { cell}, rs {row} {}
    ~TTree() {
        delete smaller;
        delete bigger;
    }

    bool insert (Cell cell, std::size_t row) {
        if (!cell) nulls.push_back (row);
        if (!c) c = cell;
        if (c == cell) rs.push_back (row);
        if (c > cell) {
            if (smaller) return smaller->insert (cell, row);
            smaller = new TTree (cell, row);
        }
        if (c < cell) {
            if (bigger) return bigger->insert (cell, row);
             bigger = new TTree (cell, row);
        }
        return true;
    }
    bool remove (Cell cell, std::size_t row) {
        if (!cell) return idx::eraseFromVector (nulls, row);
        return false;
    }
    idx::Rows select (Cell const & cell) {
        if (!cell) return nulls;
        if (c == cell) return rs;
        if (c > cell) return (smaller) ? smaller->select (cell) : idx::Rows();
        if (c < cell) return (bigger) ? bigger->select (cell) : idx::Rows();
    }
    std::vector <idx::Rows> select_if (bool (check) (Cell const & cell)) {
        if (check (Cell())) return std::vector <idx::Rows> {nulls};
        std::vector <idx::Rows> v;
        append_if (v, check);
        return std::move (v);
    }

    std::ostream & operator << (std::ostream & os) {
        return os;
    }
    std::string str() {
        return std::string();
    }
};


#endif //DATABASE_TTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */