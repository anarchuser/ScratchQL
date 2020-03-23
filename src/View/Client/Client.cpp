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
    while (true) {
        try {
            client->connectRequest().send ().wait (waitScope);
        } catch (kj::Exception & e) {
            if (e.getType() == kj::Exception::Type::DISCONNECTED) continue;
            else throw;
        }
        break;
    }
}

kj::Own <Table> Client::sendQuery (std::string const & query) {
    // Set up the request
    auto request = client->sendQueryRequest ();
    request.setQuery (query);

    // Send the request and wait for the result
    return Wrapper::unwrapTable (request.send().wait (waitScope).getTable());
}

void Client::startInterface (std::function <void (Table const &)> const & action) {
    connect();

    std::string query;
    while (true) {
        std::cout.flush();
        std::cout << "$ ";
        std::cout.flush();

        // Request input
        std::cin >> query;
        std::cout << query << std::endl;
        if (query == "exit") return;

        try {
            action (* sendQuery (query));
        } catch (std::exception & e) {
            std::cout << "SERVER: \"" << e.what() << "\"" << std::endl;
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */