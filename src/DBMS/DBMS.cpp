#include "DBMS.h"

std::unique_ptr <Table> DBMS::evalQuery (std::string const & query) {
    //TODO: Everything regarding processing of queries goes here.
    // MAKE FUNCTIONAL!!!!
    return std::make_unique <Table> (Table (std::vector <std::string>(), std::vector <KeyTypes>()));
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */