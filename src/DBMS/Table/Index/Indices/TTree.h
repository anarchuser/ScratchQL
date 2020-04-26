#ifndef DATABASE_TTREE_H
#define DATABASE_TTREE_H

#include "../../../../config.h"
#include "interface.h"
#include "Container.h"

class TTree : public IndexImpl {
private:
    using Cont = idx::Container <Cell const, std::vector <std::size_t>>;

    std::vector <std::size_t> nulls;
    Cont * root = nullptr;

public:
    TTree() = default;
    ~TTree() override { delete root; }

    bool insert (Cell cell, std::size_t row) override {
        if (!cell) {
            nulls.push_back (row);
            return true;
        }
        Cont * node = Cont::select (cell, & root);
        if (!node) return Cont::append (cell, {row}, & root);

        std::vector <std::size_t> & v = node->val;
        if (std::find (v.begin(), v.end(), row) != v.end()) {
            LOG (WARNING) << "Element to insert already exists (" << cell << "|" << row << ")!";
            return false;
        }
        v.push_back (row);
        return true;
    }
    bool remove (Cell cell, std::size_t row) override {
        if (!cell) return std::erase (nulls, row);

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


#endif //DATABASE_TTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */