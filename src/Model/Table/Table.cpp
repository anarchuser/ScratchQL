#include "Table.h"

Table::Table (std::vector <std::string> header, std::vector <KeyTypes> meta) :
    header {std::move (header)},
    meta {std::move (meta)}
{
    for (auto & key : header) {
        table.insert (std::make_pair (key, std::vector<cell>()));
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */