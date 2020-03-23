#ifndef DATABASE_SERVER_H
#define DATABASE_SERVER_H

#include "../DBMS/DBMS.h"
#include "Wrapper/Wrapper.h"

#include "generated/ServerDBMS.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>

/* Following Cap'n Proto Server example:
 * https://github.com/capnproto/capnproto/blob/master/c%2B%2B/samples/calculator-server.c%2B%2B
 */

template <class T>
class DatabaseImpl final : public RPCServer::Server {
public:
    kj::Promise <void> sendQuery (SendQueryContext context) override {
        std::cout << "Server: 'Received Query: " << context.getParams ().getQuery() << "'" << std::endl;
        auto tableBuilder = Wrapper::wrapTable (evalQuery (context.getParams().getQuery()));
        context.getResults().setTable (tableBuilder->template getRoot <RPCServer::Table>().asReader());
        return kj::READY_NOW;
    }

    kj::Promise <void> connect (ConnectContext context) override {
        std::cout << "Server: 'Listening!'" << std::endl;
        return kj::READY_NOW;
    }

private:
    kj::Own <Table const> evalQuery (std::string const & query) const {
        //TODO: make this fool proof
        return T::evalQuery (query);
    }
};

#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */