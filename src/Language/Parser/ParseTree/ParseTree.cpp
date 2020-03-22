#include "ParseTree.h"

ParseTree::ParseTree (ParseTree * parent) : parent {parent} {}

ParseTree * ParseTree::getInner() {
    if (!inner) inner = new ParseTree();
    return inner;
}
ParseTree * ParseTree::tryGetInner() const {
    return inner;
}
ParseTree * ParseTree::getNext() {
    if (!next) next = new ParseTree();
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

std::ostream & ParseTree::operator << (std::ostream & os) const {
    return (os << this->str());
}

std::string ParseTree::str (bool nl) const{
    bool isInner = parent && parent->tryGetInner();
    std::string str (token);
    if (inner) str += (isInner ? '[' : '(') + inner->str() + (isInner ? ']' : ')');
    if (next)  str += (isInner ? ',' : '.') + next->str();
    if (nl && !parent) str += '\n';
    return str;
}

char ParseTree::operator [] (int index) const {
    return token [index];
}

ParseTree::~ParseTree() {
    delete parent;
    delete inner; inner = nullptr;
    delete next;  next = nullptr;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */