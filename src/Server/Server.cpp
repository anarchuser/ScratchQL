#include "Server.h"

Server::Server (unsigned int IP, unsigned int port) :
    IP {IP},
    port {port}
{
    if (! (server_fd = socket (IP_ADDRESS_FAMILY, SOCK_STREAM, 0))) {
        LOG (ERROR) << "Couldn't create socket with Domain=" << IP_ADDRESS_FAMILY
        << ", Type=" << SOCK_STREAM
        << " and Protocol=" << 0
        << std::endl;
        throw (std::runtime_error ("Couldn't create socket"));
    }

    address.sin_family = IP_ADDRESS_FAMILY;
    address.sin_addr.s_addr = IP;
    address.sin_port = htons (port);
    memset (address.sin_zero, 0, sizeof (address.sin_zero));

    address_length = sizeof (address);

    do {
        if (!bind (server_fd, (struct sockaddr *) & address, address_length)) break;
        if (errno < -1) {
            LOG (ERROR) << "Couldn't bind to socket." << errno;
            throw (std::runtime_error ("Couldn't bind to socket"));
        }
    } while (errno == -1);

    LOG (INFO) << "Server listening on " << Server::hexToAddr (IP) << "/" << port << std::endl;

    if (::listen (server_fd, 10) < 0) {
        LOG (ERROR) << "Couldn't listen to socket";
        throw (std::runtime_error ("Couldn't listen to socket"));
    }
}

std::string Server::hexToAddr (unsigned int IP) {
    if (! IP) return std::string ("INADDR_ANY");
    if (! (IP + 1)) return std::string ("INADDR_NONE");

    std::string addr;
    for (int i = 0; i < 4; i ++) {
        if (i > 0) addr += '.';
        addr += std::to_string ((IP >> 8u * i) & 0xffu);
    }
    return addr;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */