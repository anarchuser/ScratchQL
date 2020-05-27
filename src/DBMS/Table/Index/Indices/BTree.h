#ifndef DATABASE_BTREE_H
#define DATABASE_BTREE_H

#include "../../../../config.h"
#include "interface.h"
#include "Container.h"

#include <iostream>
#include <sstream>

class BTree : public IndexImpl {
private:
    using Cont = idx::Container <Cell const, std::size_t>;

    std::vector <std::size_t> nulls;
    Cont * root = nullptr;


public:
    BTree() = default;
    ~BTree() override { delete root; }

    bool insert (Cell cell, std::size_t row) override {
        if (!!cell) return Cont::append (cell, row, & root);
        nulls.push_back (row);
        return true;
    }
    bool remove (Cell cell, std::size_t row) override {
        if (!cell) return std::erase (nulls, row);
        return Cont::remove (cell, & root);
    }
    [[nodiscard]] idx::Rows select (Cell const & cell) const override {
        if (!cell) return nulls;
        Cont * node = Cont::select (cell, & root);
        if (node) return node->val;
        LOG (WARNING) << "Selected element not found!";
        return std::monostate();
    }

    [[nodiscard]] std::string str() const override {
        if (!root) return "[empty]";
        std::stringstream ss;
        root->operator << (ss);
        return ss.str();
    }

    std::string dump() const override {
        std::stringstream ss;
        Cont::dump (ss, root, [] (std::stringstream & ss, Cell const & cell, std::size_t const & row) {
            ss << cell << '\t'
               << row  << '\n';
        });
        return ss.str();
    }
    void load (std::vector <std::pair <Cell, std::vector <std::size_t>>> data) override {
        for (auto & pair : data) {
            if (!insert (std::move (pair.first), std::move (pair.second.front()))) {
                LOG (WARNING) << "Loading of KV pair {" << pair.first << ":" << pair.second.front() << "} failed";
            }
        }
    }
    void load (std::vector <std::size_t> nulls) override {
        if (this->nulls.empty()) this->nulls = std::move (nulls);
        else for (auto & null : nulls) this->nulls.push_back (std::move (null));
    }
};


#endif //DATABASE_BTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */