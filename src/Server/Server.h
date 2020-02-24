#ifndef DATABASE_SERVER_H
#define DATABASE_SERVER_H

<<<<<<< HEAD
#include "../DBMS/DBMS.h"
||||||| constructed merge base
template <class T>
class Server {
private:
    T object;                                           // Function Object to pass requests to
=======
#include "../config.h"

#include <libnet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>

class Server {
private:
    unsigned int IP;
    unsigned int port;
    int server_fd;
    struct sockaddr_in address;
    int address_length = 0;
>>>>>>> feat(server): add basic server with few tests

<<<<<<< HEAD
#include "generated/ServerDBMS.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>

/* Following Cap'n Proto Server example:
 * https://github.com/capnproto/capnproto/blob/master/c%2B%2B/samples/calculator-server.c%2B%2B
 */
||||||| constructed merge base
public:
    explicit Server (T object) : object {object} {};
=======
public:
    Server (unsigned int IP, unsigned int port);

    template <class T>
    void listen (T && action) {
        T ();
    }
>>>>>>> feat(server): add basic server with few tests

<<<<<<< HEAD
class Server final : public RPCServer::Server {
public:
||||||| constructed merge base
    void listen (unsigned long IP, unsigned int port) {}
=======
    static std::string hexToAddr (unsigned int IP);
>>>>>>> feat(server): add basic server with few tests
};


#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */