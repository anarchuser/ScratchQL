#ifndef DATABASE_CONNECTION_HANDLER_H
#define DATABASE_CONNECTION_HANDLER_H

#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define READ_BUFFER 30000

class Connection_Handler {
private:
    std::thread thread;

public:
    Connection_Handler (int socket, std::string (query) (const char *, size_t));

    void run (int socket, std::string (query) (const char *, size_t));
};


#endif //DATABASE_CONNECTION_HANDLER_H

/* Copyright (C) 2020 Aaron Alef */