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
        return false;
    }
    [[nodiscard]] idx::Rows select (Cell const & cell) const override {
        if (!cell) return nulls;
        Cont * node = Cont::select (cell, & root);
        return (node) ? idx::Rows (node->val) : idx::Rows();
    }

    [[nodiscard]] std::string str() const override {
        std::stringstream ss;
        root->operator << (ss);
        return ss.str();
    }
};


#endif //DATABASE_BTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */