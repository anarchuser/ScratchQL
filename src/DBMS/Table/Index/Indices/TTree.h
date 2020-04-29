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
        Cont * node = Cont::select (cell, & root);
        if (!node) return false;
        std::vector const v = std::get <std::vector <std::size_t>> (select (cell));
        if (std::find (v.begin(), v.end(), row) == v.end()) {
            LOG (WARNING) << "Couldn't remove the given kv pair (" << cell << "|" << "row" << ")!";
            return false;
        }
        if (node->val.size() > 1) {
            bool erased = std::erase (node->val, row);
            if (!erased) LOG (WARNING) << "Couldn't remove the given row for element (" << cell << "|" << row << ")!";
            return erased;
        }
        return Cont::remove (cell, & root);
    }
    [[nodiscard]] idx::Rows select (Cell const & cell) const override {
        if (!cell) return nulls;
        Cont * node = Cont::select (cell, & root);
        if (node) return node->val;
        LOG (WARNING) << "Selected element (" << cell << ") can't be found!";
        return std::monostate();
    }

    [[nodiscard]] std::string str() const override {
        if (!root) return "\t[empty]";
        std::stringstream ss;
        root->operator << (ss);
        return ss.str();
    }

    virtual std::string dump() const override {
        std::stringstream ss;
        Cont::dump (ss, root, [] (std::stringstream & ss, Cell const & cell, std::vector <std::size_t > const & rows) {
            ss << cell;
            for (auto const & row : rows) ss << '\t' << row;
            ss << '\n';
        });
        return ss.str();
    }
    virtual void load (std::vector <std::pair <Cell, std::vector <std::size_t>>> & data) override {
        for (auto & pair : data) {
            std::vector <std::size_t> & v = pair.second;
            for (auto const val : v) {
                if (!insert (pair.first, val)) {
                    LOG (WARNING) << "Loading of KV pair {" << pair.first << ":" << pair.second.front() << "} failed";
                }
            }
        }
    }
};


#endif //DATABASE_TTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */