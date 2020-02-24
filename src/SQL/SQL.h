#ifndef DATABASE_SQL_H
#define DATABASE_SQL_H

#include "../config.h"
#include "Query/Query.h"

#include <memory>

namespace SQL {
    std::unique_ptr <Query> interpret (std::string const & query);
}


#endif //DATABASE_SQL_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */