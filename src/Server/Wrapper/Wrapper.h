#ifndef DATABASE_WRAPPER_H
#define DATABASE_WRAPPER_H

#include "../../DBMS/DBMS.h"
#include "../../DBMS/Table/Table.h"
#include "../generated/ServerDBMS.capnp.h"

#include <capnp/message.h>

namespace Wrapper {
    Response unwrap (::RPCServer::Maybe<::RPCServer::Table>::Reader const & reader);

    // Table
    kj::Own <capnp::MallocMessageBuilder> wrap (Response response);
    kj::Own <Table> unwrap (::RPCServer::Table::Reader const & reader);

    // Meta
    kj::Own <capnp::MallocMessageBuilder> wrap (Meta const & meta);
    Meta unwrap (RPCServer::Table::Meta::Reader const & reader);

    // Cell
    kj::Own <capnp::MallocMessageBuilder> wrap (Cell const & cell);
    Cell unwrap (RPCServer::Table::Cell::Reader const & cell);
} // Wrapper

#endif //DATABASE_WRAPPER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */