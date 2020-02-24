#ifndef DATABASE_SERVER_H
#define DATABASE_SERVER_H

template <class T>
class Server {
private:
    T object;                                           // Function Object to pass requests to

public:
    explicit Server (T object);

    void listen (unsigned long IP, unsigned int port);
};


#endif //DATABASE_SERVER_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */