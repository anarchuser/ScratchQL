#ifndef DATABASE_WRAPPER_H
#define DATABASE_WRAPPER_H

#include "../../DBMS/Table/Table.h"
#include "../generated/ServerDBMS.capnp.h"

namespace Wrapper {
    ::RPCServer::Table::Reader wrapTable (Table const & table);
    Table unwrapTable (::RPCServer::Table::Reader const & reader);

    ::RPCServer::Table::Cell::Reader wrapCell (Cell const & cell);
    Cell unwrapCell (::RPCServer::Table::Cell::Data::Reader const & field);

    namespace {
        template <class Builder, class V>
        void duplicateList (Builder & builder, V const & vector) {
            std::size_t index = 0;
            for (auto const & element : vector) builder.set (index++, element);
        }
    }
};


#endif //DATABASE_WRAPPER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */