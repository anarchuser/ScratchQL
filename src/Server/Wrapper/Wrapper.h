#ifndef DATABASE_WRAPPER_H
#define DATABASE_WRAPPER_H

#include "../../DBMS/DBMS.h"
#include "../../DBMS/Table/Table.h"
#include "../generated/ServerDBMS.capnp.h"

#include <capnp/message.h>

namespace Wrapper {
    kj::Own <capnp::MallocMessageBuilder> wrapResponse (Response response);
    Response unwrapResponse (::RPCServer::Response::Reader const & reader);

    kj::Own <capnp::MallocMessageBuilder> wrapTable (kj::Own <Table const> const & table);
    kj::Own <Table> unwrapTable (::RPCServer::Table::Reader const & reader);

    kj::Own <capnp::MallocMessageBuilder> wrapMeta (Meta const & meta);
    Meta unwrapMeta (RPCServer::Table::Meta::Reader const & reader);

    kj::Own <capnp::MallocMessageBuilder> wrapCell (Cell const & cell);
    Cell unwrapCell (RPCServer::Table::Cell::Reader const & cell);
} // Wrapper

#endif //DATABASE_WRAPPER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */