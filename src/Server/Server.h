#ifndef DATABASE_SERVER_H
#define DATABASE_SERVER_H

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

public:
    Server (unsigned int IP, unsigned int port);

    template <class T>
    void listen (T && action) {
        T ();
    }

    static std::string hexToAddr (unsigned int IP);
};


#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */