#include "../main.h"
#include "../../src/Server/Server.h"

SCENARIO ("I can run a server on a specific port") {
    GIVEN ("A server with some values") {
        Server * server;

        WHEN ("I initialise the server with some values") {
            THEN ("No error is thrown on correct usage") {
                CHECK_NOTHROW (server = new Server (INADDR_ANY, 80));
            }
        }

        delete server;
    }
}

SCENARIO ("I can convert an IP address in integer format into a string of a readable address") {
    GIVEN ("An IP address as integer") {
        std::vector <std::string> addresses = {
                "127.0.0.1",
                "255.255.255.254",
                "0.0.0.1",
                "12.34.56.78",
                "123.45.67.89",
                "0.12.43.100",
                "15.35.98.54",
                "249.76.5.8",
                "4.6.4.8",
                "4.4.8.8",
                "3.64.123.200",
                "75.43.237.97"
        };

        for (auto const & address : addresses) {
            CHECK (address == Server::hexToAddr (inet_addr (address.c_str())));
        }

        CHECK (Server::hexToAddr (INADDR_ANY) == "INADDR_ANY");
        CHECK (Server::hexToAddr (INADDR_NONE) == "INADDR_NONE");
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */