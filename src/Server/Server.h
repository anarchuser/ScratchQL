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
        capnp::MallocMessageBuilder messageBuilder;

        ::RPCServer::SendQueryResults::Builder result = messageBuilder.initRoot <::RPCServer::SendQueryResults>();
//        result.setTable (Wrapper::wrapTable (* evalQuery (context.getParams().getQuery())));

        /* Dereferencing this produces seg faults */
        evalQuery (context.getParams().getQuery());

        context.setResults (result.asReader());
        return kj::READY_NOW;
    }

    kj::Promise <void> connect (ConnectContext context) override {
        return kj::READY_NOW;
    }

private:
    std::unique_ptr <Table> const & evalQuery (std::string const & query) {
        //TODO: make this fool proof
        return T::evalQuery (query);
    }
};

#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */