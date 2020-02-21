#ifndef DATABASE_INTERPRETER_H
#define DATABASE_INTERPRETER_H

#include "../../config.h"
#include "../../Model/Table/Table.h"

#include <memory>

namespace SQL {
    std::unique_ptr <Table> interpret (std::string const & query);
}


#endif //DATABASE_INTERPRETER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */