#ifndef DATABASE_CLIENT_H
#define DATABASE_CLIENT_H

#include "../../Server/Server.h"

#include "../../Server/generated/ServerDBMS.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>
#include <thread>
#include <cmath>

/* Following Cap'n Proto Client example:
 * https://github.com/capnproto/capnproto/blob/master/c%2B%2B/samples/calculator-client.c%2B%2B
 */

class Client final {
private:
    kj::Own <capnp::EzRpcClient> ezClient;
    kj::Own <RPCServer::Client> client;
    kj::WaitScope & waitScope;

    static std::string fetchAddress (std::string const & address);
    static uint fetchPort (std::string const & address);

public:
    Client (std::string const & address, uint port);
    explicit Client (std::string const & address);

    void connect ();
    kj::Own <Table> sendQuery (std::string const & query);

    void startInterface (std::function <void (Table const &)> const & action);
};


#endif //DATABASE_CLIENT_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */