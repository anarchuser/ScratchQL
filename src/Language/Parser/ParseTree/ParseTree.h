#ifndef DATABASE_PARSETREE_H
#define DATABASE_PARSETREE_H

#include <iostream>
#include <string>

struct ParseTree {
private:
    ParseTree * parent;
    ParseTree * inner = nullptr;
    ParseTree * next = nullptr;

public:
    explicit ParseTree (ParseTree * parent = nullptr);
    ~ParseTree();

    std::string token;

    ParseTree * getInner();
    [[nodiscard]] ParseTree * tryGetInner() const;

    ParseTree * getNext();
    [[nodiscard]] ParseTree * tryGetNext() const;

    [[nodiscard]] std::string getTokenName() const;
    [[nodiscard]] std::string str (bool nl = false) const;

    std::ostream & operator << (std::ostream & os) const;
    [[nodiscard]] char operator [] (int index) const;
};



#endif //DATABASE_PARSETREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */