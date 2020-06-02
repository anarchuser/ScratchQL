#include "Client.h"

Client::Client (std::string const & address, uint port) :
        ezClient {kj::heap <capnp::EzRpcClient> (address, port)},
        client {kj::heap <RPCServer::Client> (ezClient->getMain <RPCServer>())},
        waitScope {ezClient->getWaitScope()} {}

Client::Client (std::string const & address) :
        ezClient {kj::heap <capnp::EzRpcClient> (fetchAddress (address), fetchPort (address))},
        client {kj::heap <RPCServer::Client> (ezClient->getMain <RPCServer>())},
        waitScope {ezClient->getWaitScope()} {}

std::string Client::fetchAddress (std::string const & address) {
    std::string newAddress;
    for (char c : address) {
        if (c == ':') break;
        newAddress += c;
    }
    return newAddress;
}

uint Client::fetchPort (std::string const & address) {
    std::string port;
    uint newPort = 0;
    int factor = 1;
    bool isPort = false;
    for (char c : address) {
        if (isPort) port += c;
        if (c == ':') isPort = true;
    }
    for (std::size_t i = port.size(); i > 0; --i) {
        newPort += (port [i] - '0')  * factor;
        factor *= 10;
    }
    return newPort;
}

void Client::connect () {
    client->connectRequest().send ().wait (waitScope);
}

Response Client::sendQuery (std::string const & query) {
    // Set up the request
    auto request = client->sendQueryRequest ();
    request.setQuery (query);

    // Send the request and wait for the result
    return Wrapper::unwrapResponse (request.send().wait (waitScope).getResponse());
}

void Client::startInterface (std::function <void (Response)> const & action) {
    connect();

    std::string query;
    while (true) {
        (std::cout << "$ ").flush();

        // Request input
        getline (std::cin, query);
        bool pass = false;
        for (char c : query) if (c != ' ' && c != '\n') pass = true;
        if (!pass || query.empty()) continue;

        std::cout << query << std::endl;
        if (query == "exit") return;

        try {
            action (sendQuery (query));
        } catch (std::exception & e) {
            std::cout << "Server: '" << e.what() << "'" << std::endl;
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */