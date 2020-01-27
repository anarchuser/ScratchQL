#include <netinet/in.h>
#include "Server.h"

Server::Server (int port) : port{port} {
    if (! (server_fd = socket (ADDR_FAMILY, SOCK_STREAM, 0))) {
        throw (std::runtime_error ("Couldn't create socket"));
    }

    address.sin_family = ADDR_FAMILY;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (port);
    memset (address.sin_zero, 0, sizeof (address.sin_zero));

    addr_len = sizeof (address);

    if (bind (server_fd, (struct sockaddr *) & address, sizeof (address)) < 0) {
        throw (std::runtime_error ("Couldn't bind socket"));
    }

    if (listen (server_fd, 10) < 0) {
        throw (std::runtime_error ("Couldn't listen to socket"));
    }
}

void Server::run() {
    while (true) {
        int socket = accept (server_fd, (struct sockaddr *) & address, (socklen_t *) & addr_len);
        if (socket < 0) {
            throw (std::runtime_error ("Couldn't accept connection"));
        }
        new Connection_Handler (socket, mock);
    }
}

/* Copyright (C) 2020 Aaron Alef */