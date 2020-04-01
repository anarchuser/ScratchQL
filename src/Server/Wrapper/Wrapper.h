#ifndef DATABASE_WRAPPER_H
#define DATABASE_WRAPPER_H

#include "../../DBMS/DBMS.h"
#include "../../DBMS/Table/Table.h"
#include "../generated/ServerDBMS.capnp.h"

#include <capnp/message.h>

namespace Wrapper {

kj::Own <capnp::MallocMessageBuilder> const & wrapResponse (Response response);
Response unwrapResponse (::RPCServer::Response::Reader const & reader);

kj::Own <capnp::MallocMessageBuilder> wrapTable (kj::Own <Table const> const & table);
kj::Own <Table> unwrapTable (::RPCServer::Table::Reader const & reader);

kj::Own <capnp::MallocMessageBuilder> wrapCell (Cell const & cell);
Cell unwrapCell (RPCServer::Table::Cell::Reader const & cell);

inline RPCServer::Table::Cell::Reader _wrapCell (Cell const & cell) {
    return wrapCell (cell)->getRoot <RPCServer::Table::Cell>();
}

namespace {
template <class Builder, class V>
void duplicateList (Builder & builder, V const & vector) {
    std::size_t index = 0;
    for (auto const & element : vector) builder.set (index++, element);
}

} // private Wrapper
} // Wrapper

#endif //DATABASE_WRAPPER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */