#ifndef DATABASE_TTREE_H
#define DATABASE_TTREE_H

#include "../../../../config.h"
#include "interface.h"

class TTree : public IndexImpl {
private:
    using Cont = idx::Container <std::pair <Cell, std::vector <std::size_t>>>;

    std::vector <std::size_t> nulls;
    Cont * root = nullptr;

public:
    TTree() = default;
    ~TTree() { delete root; }

    bool insert (Cell cell, std::size_t row) {
        if (!cell) {
            nulls.push_back (row);
            return true;
        }
        Cont *   node_ptr =   root;
        Cont * * trav_ptr = & root;
        while (* trav_ptr) {
            node_ptr = * trav_ptr;
            Cell const & stored = node_ptr->val.first;
            if (cell == stored) {
                auto & v = node_ptr->val.second;
                if (std::find (v.begin(), v.end(), row) != v.end()) {
                    LOG (WARNING) << "Cell already exists in Tertiary Tree with unique indices";
                    return false;
                } else {
                    v.push_back (row);
                    return true;
                }
            }
            auto tmp = * trav_ptr;
            trav_ptr = (cell < stored) ? & node_ptr->smaller : & node_ptr->bigger;
            if (tmp == * trav_ptr) LOG (FATAL) << "Circular dependency in tree detected!";
        }
        return * trav_ptr = new Cont ({cell, {row}}, node_ptr);
    }
    bool remove (Cell cell, std::size_t row) {
        if (!cell) return std::erase (nulls, row);
        Cont * * trav_ptr = & root;
        auto & stored = (* trav_ptr)->val;
        while (stored.first != cell) {
            if (cell < stored.first) trav_ptr = & (* trav_ptr)->smaller;
            if (cell > stored.first) trav_ptr = & (* trav_ptr)->bigger;
            if (!trav_ptr) return false;
            stored = (* trav_ptr)->val;
        }
        if (stored.second.size() > 1) {
            if (std::find (stored.second.begin(), stored.second.end(), row) == stored.second.end()) {
                LOG (WARNING) << "Trying to remove a non-existing element!";
            }
            return std::erase (stored.second, row);
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
    idx::Rows select (Cell const & cell) const {
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

    std::string str() const {
        std::stringstream ss;
        root->operator << (ss);
        return ss.str();
    }
};


#endif //DATABASE_TTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */