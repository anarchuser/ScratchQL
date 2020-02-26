#ifndef DATABASE_CLIENT_H
#define DATABASE_CLIENT_H

#include "../../Server/Server.h"

#include "../../Server/generated/ServerDBMS.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>

/* Following Cap'n Proto Client example:
 * https://github.com/capnproto/capnproto/blob/master/c%2B%2B/samples/calculator-client.c%2B%2B
 */

class Client final : public RPCServer::Server {
public:
    kj::Promise<void> sendQuery (SendQueryContext context) override {
        auto params = context.getParams().getQuery();
        context.getResults().getTable();
        return kj::READY_NOW;
    }
};


#endif //DATABASE_CLIENT_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */