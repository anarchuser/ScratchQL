#ifndef DATABASE_TTREE_H
#define DATABASE_TTREE_H

#include "../../../../config.h"
#include "interface.h"

class TTree : public IndexImpl {
private:

public:
    TTree () = default;

    bool insert (Cell cell, std::size_t row) {}
    bool remove (Cell cell, std::size_t row) {}
    idx::Rows select (Cell const & cell) {}
    std::vector <idx::Rows> const & select_if (bool (check) (Cell const & cell)) {}
};


#endif //DATABASE_TTREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */