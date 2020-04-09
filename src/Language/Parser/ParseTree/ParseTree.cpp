#include "ParseTree.h"

ParseTree::ParseTree (ParseTree * parent) : parent {parent} {}
ParseTree::ParseTree() : ParseTree (nullptr) {};

ParseTree * ParseTree::getInner() {
    if (!inner) inner = new ParseTree (this);
    return inner;
}
ParseTree * ParseTree::tryGetInner() const {
    return inner;
}
ParseTree * ParseTree::getNext() {
    if (!next) next = new ParseTree (this);
    return next;
}
ParseTree * ParseTree::tryGetNext() const {
    return next;
}

std::string ParseTree::getTokenName() const {
    std::string ns;
    for (char c : token) if (std::isalpha (c)) ns += c;
    return ns;
}

Token::Type ParseTree::getParentType() const {
    return parent ? parent->type : Token::VOID;
}

std::ostream & ParseTree::operator << (std::ostream & os) const {
    return (os << this->str());
}

std::string const & ParseTree::str (std::string & s, bool pp) const {
    if (pp && parent) return parent->str (s, true);
    bool isParam = parent && parent->tryGetInner ();
    s += token;
    if (inner) {
        s += (isParam ? type == Token::KV_PAIR ? '{' : '[' : '(');
        inner->str (s, false);
        s += (isParam ? type == Token::KV_PAIR ? '}' : ']' : ')');
    }
    if (next) {
        s += (isParam ? type == Token::KEY ? ',' : ':' : '.');
        next->str (s, false);
    }
    return s;
}

std::string ParseTree::str (bool pp) const {
    std::string string;
    return str (string, pp);
}

char ParseTree::operator [] (int index) const {
    return token [index];
}

ParseTree::~ParseTree() {
    delete inner;
    delete next;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */