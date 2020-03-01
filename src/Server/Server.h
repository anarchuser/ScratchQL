#ifndef DATABASE_SERVER_H
#define DATABASE_SERVER_H

#include "../DBMS/DBMS.h"

#include "generated/ServerDBMS.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>

/* Following Cap'n Proto Server example:
 * https://github.com/capnproto/capnproto/blob/master/c%2B%2B/samples/calculator-server.c%2B%2B
 */

class DatabaseImpl final : public RPCServer::Server {
public:
    kj::Promise <void> sendQuery (SendQueryContext context) override {
        auto params = context.getParams();
//        context.getResults().setTable (convert (evalQuery (params.getQuery())));
        return kj::READY_NOW;
    }

    kj::Promise <void> connect (ConnectContext context) override {
        return kj::READY_NOW;
    }

private:
    ::RPCServer::Table::Reader convert (Table const & table) {}

    Table const & evalQuery (std::string const & query) {}
};

#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */