#include "Parser.h"

kj::Own <Query> Parser::parseQuery (std::string const & rawQuery) {
    validateQuery (rawQuery);
    std::string despacedQuery = despace (rawQuery);
    std::string tokenisedQuery = tokenise (despacedQuery);
    kj::Own <Query> procQuery = buildQuery (tokenisedQuery);

    return procQuery;
}

void Parser::validateQuery (std::string const & text) {
    for (auto character : text) validateChar (character);
}

void Parser::validateChar (char c) {
    if (std::isalnum (c)) return;
    for (auto validChar : validQueryChars) if (c == validChar) return;

    THROW (std::logic_error (STR+ "Malformed Query (invalid char detected): " + c + "[" + std::to_string ((short) c) + "])"));
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
            THROW (std::logic_error (STR+ "Unexpected Token before '(': " + c + " (letter expected)"));
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
}

kj::Own <Query> Parser::buildQuery (std::string const & query) {
    kj::Own <Query> structure = kj::heap <Query>();
    std::string::const_iterator iterator = query.begin();

    // Set database name and action
    if (* iterator == '$') {
        copyToken (& ++iterator, structure->database);
        if (* iterator != '.') {
            THROW (std::logic_error (STR+
            "Unexpected symbol found: " + * iterator + " (expected . to separate tokens)"));
        } else iterator++;
        if (* iterator == '$') {
            if (* (iterator + 1) == '@') structure->targetType = Database::Target::USER;
            else if (* (iterator + 1) == '#') structure->targetType = Database::Target::TABLE;
            else {
                THROW (std::logic_error (STR+
                "Ambiguous statement found. Please annotate targets with @ for users and # for tables"));
            }
            copyToken (& ++iterator, structure->target);
            structure->actionOnTarget = Database::Target::WRITE;
            if (structure->targetType == Database::Target::TABLE) {
                auto spec = Database::Target::Table::Specification ();
                spec.action = (Database::Target::Table::Action) lookUpEnum (& ++iterator, Database::Target::Table::ActionStrings);
                structure->spec = std::move (Database::Target::Specification (spec));
            } else {
                auto spec = Database::Target::User::Specification ();
                spec.action = (Database::Target::User::Action) lookUpEnum (& ++iterator, Database::Target::User::ActionStrings);
                structure->spec = std::move (Database::Target::Specification (spec));
            }
        } else if (std::isalpha (* iterator)){
            structure->actionOnTarget = (Database::Target::Action)
                    lookUpEnum (& iterator, Database::Target::ActionStrings);
        } else {
            THROW (std::logic_error (STR+
            "Invalid symbol found: " + * iterator + " (Action or database expected"));
        }
    } else if (std::isalpha (* iterator)) {   // Assume Add / Delete action:
        structure->actionOnDatabase = (Database::Action) lookUpEnum (& iterator, Database::ActionStrings);
        if (* iterator != '(') {
            THROW (std::logic_error (STR+
            "Unexpected symbol found: " + * iterator + " (expected '(' with function call parameters)"));
        } else iterator++;
        if (* iterator == '$') copyToken (& iterator, structure->database);
        return structure;
    } else {
        THROW (std::logic_error (STR+
        "Invalid symbol found: " + * iterator + " (Action or database expected"));
    }

    return structure;
}

void Parser::copyToken (std::string::const_iterator * source, std::string & target) {
    while (std::isalpha (* * source)) target += * (* source)++;
}

short Parser::lookUpEnum (std::string::const_iterator * source, std::vector <std::string> const & enums) {
    std::string enumString;
    copyToken (source, enumString);

    short index = 0;
    for (auto const & e : enums) {
        if (e == enumString) return index;
        index++;
    }
    THROW (std::logic_error (STR+ "Invalid Keyword found: " + enumString));
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */