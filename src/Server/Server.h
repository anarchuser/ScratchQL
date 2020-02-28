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

class Server final : public RPCServer::Server {
public:
};


#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */