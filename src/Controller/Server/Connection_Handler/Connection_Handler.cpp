#include "Connection_Handler.h"

Connection_Handler::Connection_Handler (int socket, std::string (query) (const char *, size_t)) :
    thread(& Connection_Handler::run, this, socket, query)
{
    delete this;
}

Connection_Handler::~Connection_Handler () {
    thread.join();
}

void Connection_Handler::run (int socket, std::string (query) (const char *, size_t)) {
    char buffer [READ_BUFFER] = {0};
    size_t val_read = read(socket, buffer, READ_BUFFER);

    std::string response = query (buffer, val_read);
    write (socket, & response [0], response.size());

    close (socket);
}

/* Copyright (C) 2020 Aaron Alef */