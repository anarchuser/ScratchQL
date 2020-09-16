#include "Column.h"

qy::Column::Column (Table parent, std::string name) :
        name {std::move(name)},
        parent {std::move(parent)} {}

/* Copyright (C) 2020 Aaron Alef */