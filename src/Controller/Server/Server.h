/* Referencing this Medium article:
 * https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
 */

#ifndef DATABASE_SERVER_H
#define DATABASE_SERVER_H

#include "../Controller.h"
#include "Connection_Handler/Connection_Handler.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <stdexcept>
#include <cstring>

#define ADDR_FAMILY AF_INET

class Server {
private:
    int server_fd;
    struct sockaddr_in address;
    int port;
    int addr_len = 0;

public:
    explicit Server (int port);

    void run ();
};


#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef */