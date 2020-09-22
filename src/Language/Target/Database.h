#ifndef DATABASE_DATABASE_H
#define DATABASE_DATABASE_H

#include <string>
#include <utility>

namespace qy {
    struct Database {
        explicit Database (std::string name);

        std::string name;

        bool operator == (Database const & other) const;
        bool operator != (Database const & other) const;
    };
}


#endif //DATABASE_DATABASE_H

/* Copyright (C) 2020 Aaron Alef */