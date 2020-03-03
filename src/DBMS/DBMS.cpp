#include "DBMS.h"

kj::Own <Table const> DBMS::evalQuery (std::string const & query) {
    //TODO: Everything regarding processing of queries goes here.
    // MAKE FUNCTIONAL!!!!
    return kj::heap <Table> (std::vector <std::string>(), std::vector <KeyTypes>());
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */