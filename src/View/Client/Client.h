#ifndef DATABASE_CLIENT_H
#define DATABASE_CLIENT_H

#include "../../Server/Server.h"

#include "../../Server/generated/ServerDBMS.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>
#include <thread>

/* Following Cap'n Proto Client example:
 * https://github.com/capnproto/capnproto/blob/master/c%2B%2B/samples/calculator-client.c%2B%2B
 */

class Client final {
private:
    capnp::EzRpcClient * ezClient;
    RPCServer::Client * client;
    kj::WaitScope & waitScope;

    void connect ();

    Table const & sendQuery (std::string const & query);
    Table const & extractTable (::RPCServer::Table::Reader const & tableReader);

public:
    explicit Client (std::string const & address, uint port);
    ~Client();

    void startInterface (std::function <void (Table const &)> const & action);
};


#endif //DATABASE_CLIENT_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */