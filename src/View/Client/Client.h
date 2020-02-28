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
private:
    kj::Promise <void> sendQuery (SendQueryContext context) override {
        auto params = context.getParams().getQuery();
        KJ_REQUIRE (params.size() == 1, "Wrong number of parameters");
        context.getResults().getTable();
        return kj::READY_NOW;
    }

public:
    void connect(std::string address) {
        capnp::EzRpcClient client (address);
        RPCServer::Client CLIent = client.getMain <RPCServer>();

        auto & waitScope = client.getWaitScope ();

        std::string query;
        while (true) {
            std::cout << ">>> ";
            std::cout.flush();

            // Request input
            std::cin >> query;
            if (query == "exit") return;

            // Set up the request
            auto request = CLIent.sendQueryRequest ();
            request.setQuery (query);

            // Send the request and wait for the result
            auto response = request.send().wait (waitScope);
        }
    }
};


#endif //DATABASE_CLIENT_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */