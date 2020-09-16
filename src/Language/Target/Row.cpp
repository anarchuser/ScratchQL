#include "Row.h"

qy::Row::Row (Table parent, std::vector <Column> columns, std::vector <Specification> specs) :
        parent {std::move(parent)},
        columns {std::move(columns)},
        specs {std::move(specs)} {}

/* Copyright (C) 2020 Aaron Alef */