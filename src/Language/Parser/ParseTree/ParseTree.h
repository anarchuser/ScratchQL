#ifndef DATABASE_PARSETREE_H
#define DATABASE_PARSETREE_H

#include <iostream>
#include <string>

namespace Token {
    enum Type { VOID, FUNCTION, DATABASE, USER, TABLE, LIST, KV_PAIR, KEY, VALUE };
}

struct ParseTree {
private:
    ParseTree * parent;
    ParseTree * inner = nullptr;
    ParseTree * next = nullptr;

    explicit ParseTree (ParseTree * parent);

    std::string const & str (std::string & str, bool printParents = true) const;

public:
    ParseTree();
    ~ParseTree();

    std::string token;
    Token::Type type = Token::VOID;

    ParseTree * getInner();
    [[nodiscard]] ParseTree * tryGetInner() const;

    ParseTree * getNext();
    [[nodiscard]] ParseTree * tryGetNext() const;

    [[nodiscard]] Token::Type getParentType() const;

    [[nodiscard]] std::string getTokenName() const;
    [[nodiscard]] std::string str (bool printParents = true) const;

    std::ostream & operator << (std::ostream & os) const;
    [[nodiscard]] char operator [] (int index) const;
};



#endif //DATABASE_PARSETREE_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */