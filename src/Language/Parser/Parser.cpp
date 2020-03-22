#include "Parser.h"

kj::Own <Query> Parser::parseQuery (std::string const & rawQuery) {
    validateQuery (rawQuery);
    std::string despacedQuery = despace (rawQuery);
    std::string enrichedQuery = enrich (despacedQuery);
    kj::Own <ParseTree> tokenisedQuery = tokeniseQuery (enrichedQuery);
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

bool Parser::isWordChar (char c) {
    return std::isalpha (c) || c == '@' || c == '#' || c == '$';
}

std::string Parser::despace (std::string const & text) {
    std::string despaced;
    for (char cha : text) if (cha != ' ' && cha != '\n') despaced += cha;
    return std::move (despaced);
}

std::string Parser::enrich (std::string const & text) {
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

        if (isWordChar (c)) isWord = true;
        if (isKeyWord && !isWord) {
            THROW (std::logic_error (STR+ "Unexpected Token before '(': " + c + " (letter expected)"));
        }
        if (isWord && !isWordChar (c)) {
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

kj::Own <ParseTree> Parser::tokeniseQuery (std::string const & query) {
    kj::Own <ParseTree> tokens = kj::heap <ParseTree>();
    std::string::const_iterator iterator = query.begin();
    readToken (& iterator, query.end(), tokens);
    return tokens;
}

void Parser::readToken (std::string::const_iterator * source, std::string::const_iterator end, ParseTree * tree) {
    bool isString = false;
    while (* source < end) {
        char c = * (* source)++;
        if (c == '"') isString = !isString;
        if (c == '"' || isString) {
            tree->token += c;
            continue;
        }
        if (CLOSE_PAREN (c)) {
            ++ * source;
            return;
        }
        if (OPEN_PAREN (c)) {
            readToken (source, end, tree->getInner());
            continue;
        }
        if (SEPARATOR (c)) {
            readToken (source, end, tree->getNext());
            continue;
        }
        if (!isWordChar (c)) {
            THROW (std::logic_error (STR+
            "Unexpected symbol found: " + c + " (word or separator expected)"));
        }
        tree->token += c;
    }
}

kj::Own <Query> Parser::buildQuery (kj::Own <ParseTree> const & pt) {
    kj::Own <Query> structure = kj::heap <Query>();
    ParseTree const * token = & * pt;

    // Set database name and action
    if ((* token) [0] == '$') {
        structure->database = token->getTokenName();
        structure->actionOnDatabase = Database::CHANGE;
        LOG_ASSERT (!token->tryGetInner());
        if (!token->tryGetNext()) {
            THROW (std::logic_error (STR+
            "Method on Database '" + structure->database + "' expected; found nothing"));
        }
        token = token->tryGetNext();
        if ((* token) [0] == '$') {
            if ((* token) [1] == '@') structure->targetType = Database::Target::USER;
            else if ((* token) [1] == '#') structure->targetType = Database::Target::TABLE;
            else {
                THROW (std::logic_error (STR+
                "Ambiguous statement found. Please prefix targets with @ for users and # for tables"));
            }
            structure->target = token->getTokenName();
            structure->actionOnTarget = Database::Target::CHANGE;
            LOG_ASSERT (!token->tryGetInner());
            if (!token->tryGetNext()) {
                THROW (std::logic_error (STR+
                "Method on Target '" + structure->target + "' expected; found nothing"));
            }
            token = token->tryGetNext();
            if (structure->targetType == Database::Target::TABLE) {
                auto spec = Database::Target::Table::Specification ();
                spec.action = (Database::Target::Table::Action) lookUpEnum (token->getTokenName(), Database::Target::Table::ActionStrings);
                structure->spec = std::move (Database::Target::Specification (spec));
            } else {
                auto spec = Database::Target::User::Specification ();
                spec.action = (Database::Target::User::Action) lookUpEnum (token->getTokenName(), Database::Target::User::ActionStrings);
                structure->spec = std::move (Database::Target::Specification (spec));
            }
        } else if (std::isalpha ((* token) [0])){
            structure->actionOnTarget = (Database::Target::Action)
                    lookUpEnum (token->getTokenName(), Database::Target::ActionStrings);
            LOG_ASSERT (!token->tryGetNext());
            if (!token->tryGetInner()) {
                THROW (std::logic_error (STR +
                "Function call parameters for '" + token->getTokenName() + "' expected; nothing found"));
            }
            token = token->tryGetInner();
            if ((* token) [0] == '$') {
                if ((* token) [1] == '@') structure->targetType = Database::Target::USER;
                else if ((* token) [1] == '#') structure->targetType = Database::Target::TABLE;
                else {
                    THROW (std::logic_error (STR+
                    "Ambiguous statement found. Please annotate targets with @ for users and # for tables"));
                }
                structure->target = token->getTokenName();
            } else {
                THROW (std::logic_error (STR+
                "Function call parameter for '" + token->getTokenName() + "' expected; function found"));
            }
        } else {
            THROW (std::logic_error (STR+
            "Invalid symbol found: '" + token->getTokenName() + "' (Action or database expected"));
        }
    } else {
        structure->actionOnDatabase = (Database::Action) lookUpEnum (token->getTokenName(), Database::ActionStrings);
        if (! token->tryGetInner()) {
            THROW (std::logic_error (STR +
            "Missing function parameters to call: '" + token->getTokenName() + "'"));
        }
        if (structure->actionOnDatabase == Database::Action::DATABASES ||
            structure->actionOnDatabase == Database::Action::USERS)
            return structure;
        token = token->tryGetInner();
        if ((* token) [0] == '$') structure->database = token->getTokenName();
        else {
            THROW (std::logic_error (STR +
            "Expected function parameter; found function: '" + token->getTokenName() + "'"));
        }
        return structure;
    }
    return structure;
}

short Parser::lookUpEnum (std::string const & str, std::vector <std::string> const & enums) {
    short index = 0;
    for (auto const & e : enums) {
        if (str == e) return index;
        index++;
    }
    THROW (std::logic_error (STR+ "Invalid Keyword found: '" + str + "'"));
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */