#include "Parser.h"

kj::Own <Query> Parser::parseQuery (std::string const & rawQuery) {
    kj::Own <Query> procQuery = kj::heap <Query>();

    validateQuery (rawQuery);
    std::string despacedQuery = despace (rawQuery);
    std::string tokenisedQuery = tokenise (despacedQuery);

    for (auto const & token : rawQuery) {

    }

    return procQuery;
}

void Parser::validateQuery (std::string const & text) {
    for (auto character : text) validateChar (character);
}

void Parser::validateChar (char c) {
    if (std::isalnum (c)) return;
    for (auto validChar : validQueryChars) if (c == validChar) return;

    THROW (std::logic_error (std::string() +
    "Malformed Query (invalid char detected: " + c + "[" + std::to_string ((short) c) + "])"
    ));
}

std::string Parser::despace (std::string const & text) {
    std::string despaced;
    for (char cha : text) if (cha != ' ') despaced += cha;
    return std::move (despaced);
}

std::string Parser::tokenise (std::string const & text) {
    std::string swap, tokenised;
    bool isWord = false;
    bool isKeyWord = false;
    bool isString = false;

    for (std::size_t index = text.size(); --index < (std::size_t) -1;) {
        char c = text [index];

        if (c == '"') isString = !isString;
        if (isString) {
            tokenised += c;
            continue;
        }

        if (std::isalpha (c)) isWord = true;
        if (isKeyWord && !isWord) {
            THROW (std::logic_error ("Unexpected Token before '(': letter expected"));
        }
        if (isWord && !std::isalpha (c)) {
            if (!isKeyWord) tokenised += '$';
            isWord = isKeyWord = false;
        }
        if (c == '(') isKeyWord = true;
        tokenised += isKeyWord ? std::toupper (c) : c;
    }
    if (isWord && !isKeyWord) tokenised += '$';

    for (std::size_t index = tokenised.size(); --index < (std::size_t) -1;) swap += tokenised [index];
    return swap;
};

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */