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

    virtual std::string dump() const override {
        std::stringstream os;
//        for (auto const & pair : map) {
//            os << pair.first;
//            for (auto const row : pair.second) os << '\t' << row;
//            os << '\n';
//        }
        return os.str();
    }
    virtual void load (std::vector <std::pair <Cell, std::vector <std::size_t>>> & data) override {

    }
};


#endif //DATABASE_BTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */