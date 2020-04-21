#ifndef DATABASE_BTREE_H
#define DATABASE_BTREE_H

#include "../../../../config.h"
#include "interface.h"
#include "Container.h"

class BTree : public IndexImpl {
private:
    using Cont = idx::Container <std::pair <Cell, std::size_t>>;

    std::vector <std::size_t> nulls;
    Cont * root = nullptr;

    void append_if (std::vector <idx::Rows> rows, Cont * node, bool (check) (Cell const & cell)) {
        if (!node) return;
        if (check (node->val.first)) rows.emplace_back (node->val.second);
        append_if (rows, node->smaller, check);
        append_if (rows, node->bigger,  check);
    }

public:
    BTree() = default;
    ~BTree() { delete root; }

    bool insert (Cell cell, std::size_t row) {
        if (!cell) {
            nulls.push_back (row);
            return true;
        }
        Cont * node_ptr = root;
        Cont * * trav_ptr = & root;
        while (* trav_ptr) {
            Cell const & stored = (* trav_ptr)->val.first;
            if (cell == stored) return false;
            node_ptr = * trav_ptr;
            if (cell < stored) trav_ptr = & (* trav_ptr)->smaller;
            else trav_ptr = & (* trav_ptr)->bigger;
        }
        return * trav_ptr = new Cont ({cell, row}, node_ptr);
    }
    bool remove (Cell cell, std::size_t row) {
        if (!cell) return idx::eraseFromVector (nulls, row);
        Cont * * trav_ptr = & root;
        auto & stored = (* trav_ptr)->val;
        while (stored.first != cell) {
            if (cell < stored.first) trav_ptr = & (* trav_ptr)->smaller;
            if (cell > stored.first) trav_ptr = & (* trav_ptr)->bigger;
            if (!trav_ptr) return false;
            stored = (* trav_ptr)->val;
        }
        Cont * oldNode = * trav_ptr;
        Cont * branch = (* trav_ptr)->smaller;
        (* trav_ptr)->parent = * trav_ptr;
        * trav_ptr = (* trav_ptr)->bigger;

        oldNode->smaller = nullptr;
        oldNode->bigger  = nullptr;
        oldNode->parent  = nullptr;
        delete oldNode;

        while (* trav_ptr) trav_ptr = & (* trav_ptr)->smaller;
        return * trav_ptr = branch;
    }
    idx::Rows select (Cell const & cell) {
        if (!cell) return nulls;
        Cont * trav = root;
        auto & stored = trav->val;
        while (stored.first != cell) {
            if (cell < stored.first) trav = trav->smaller;
            if (cell > stored.first) trav = trav->bigger;
            if (!trav) return false;
            stored = trav->val;
        }
        return stored.second;
    }
    std::vector <idx::Rows> select_if (bool (check) (Cell const & cell)) {
        if (check (Cell())) return std::vector <idx::Rows> {nulls};
        std::vector <idx::Rows> rows;
        append_if (rows, root, check);
        return std::move (rows);
    }

    std::ostream & operator << (std::ostream & os) {
        return os;
    }
    std::string str() {
        return std::string();
    }
};


#endif //DATABASE_BTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */