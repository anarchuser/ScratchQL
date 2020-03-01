#include "Client.h"

Client::Client (std::string const & address, uint port) :
        ezClient {new capnp::EzRpcClient (address, port)},
        client {new RPCServer::Client (ezClient->getMain <RPCServer>())},
        waitScope {ezClient->getWaitScope()} {}

Client::~Client() {
    delete ezClient;
    delete client;
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

Table Client::sendQuery (std::string const & query) {
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
        std::cout << ">>> ";
        std::cout.flush();

        // Request input
        std::cin >> query;
        if (query == "exit") return;

        action (sendQuery (query));
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */