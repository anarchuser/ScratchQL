#ifndef DATABASE_DBMS_H
#define DATABASE_DBMS_H

#include "FileHandler/FileHandler.h"
#include "Table/Table.h"
#include "../Language/Target/Target.h"
#include "../Language/Target/Table.h"
#include "../Language/Target/Row.h"
#include "../Language/Parser/Parser.h"
#include "../Util/Tokens.h"
#include "../Util/Helper.h"

#include <memory>
#include <kj/async.h>
#include <optional>
#include <filesystem>

typedef std::optional <kj::Own <Table const>> Response;
std::ostream & operator << (std::ostream & os, Response const & response);

struct DBMS {
    DBMS() = delete;

    static void  create (Target const & target);
    static Table select (Target const &  target);
    static void  modify (qy::Row const & target, std::vector <Cell> const & data);
    static void  insert (qy::Table const & target, std::vector <Cell> const & data);
    static void  remove (Target const & target);
};


#endif //DATABASE_DBMS_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */