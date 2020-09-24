#ifndef DATABASE_WRAPPER_H
#define DATABASE_WRAPPER_H

#include "../../DBMS/DBMS.h"
#include "../../DBMS/Table/Table.h"
#include "../../Language/Target/Target.h"
#include "../generated/ServerDBMS.capnp.h"

#include <capnp/message.h>

namespace Wrapper {
    // Table
    kj::Own <capnp::MallocMessageBuilder> wrap (Table const & table);
    kj::Own <Table> unwrap (::RPCServer::Table::Reader reader);

    // Meta
    kj::Own <capnp::MallocMessageBuilder> wrap (Meta const & meta);
    Meta unwrap (RPCServer::Table::Meta::Reader reader);

    // Cell
    kj::Own <capnp::MallocMessageBuilder> wrap (Cell const & cell);
    Cell unwrap (RPCServer::Table::Cell::Reader cell);

    // Target
    kj::Own <capnp::MallocMessageBuilder> wrap (Target const & target);
    Target unwrap (::RPCServer::Target::Reader reader);

    qy::Database      unwrap (::RPCServer::Target::Database::Reader           reader);
    qy::Table         unwrap (::RPCServer::Target::Table::Reader              reader);
    qy::Column        unwrap (::RPCServer::Target::Column::Reader             reader);
    qy::Row           unwrap (::RPCServer::Target::Row::Reader                reader);
    qy::Specification unwrap (::RPCServer::Target::Row::Specification::Reader reader);
    std::vector <qy::Column> unwrap (capnp::List <::RPCServer::Target::Column>::Reader reader);
    std::vector <qy::Specification> unwrap (capnp::List <::RPCServer::Target::Row::Specification>::Reader reader);
    std::vector <Cell> unwrap (capnp::List <::RPCServer::Table::Cell>::Reader reader);

    struct TargetVisitor {
        explicit TargetVisitor (RPCServer::Target::Builder builder);

        void operator ()(qy::Database const & target);
        void operator ()(qy::Table const & target);
        void operator ()(qy::Column const & target);
        void operator ()(qy::Row const & target);

    private:
        RPCServer::Target::Builder builder;
    };

    void init (RPCServer::Target::Database::Builder builder, qy::Database const & target);
    void init (RPCServer::Target::Table::Builder builder, qy::Table const & target);
    RPCServer::Target::Column::Reader init (RPCServer::Target::Column::Builder builder, qy::Column const & target);
    void init (RPCServer::Target::Row::Builder builder, qy::Row const & target);
    RPCServer::Target::Row::Specification::Reader init (RPCServer::Target::Row::Specification::Builder builder, qy::Specification const & spec);

} // Wrapper

#endif //DATABASE_WRAPPER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */