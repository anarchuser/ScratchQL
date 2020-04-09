#include "Parser.h"

kj::Own <Query> Parser::parseQuery (std::string const & rawQuery) {
    validateQuery (rawQuery);
    std::string despacedQuery = despace (rawQuery);
    std::string enrichedQuery = enrich (despacedQuery);
    kj::Own <ParseTree> tokenisedQuery = tokeniseQuery (enrichedQuery);
    kj::Own <Query> procQuery = buildQuery (tokenisedQuery);

    return procQuery;
}

inline void Parser::validateQuery (std::string const & text) {
    for (auto character : text) validateChar (character);
}

inline void Parser::validateChar (char c) {
    if (std::isalnum (c)) return;
    for (auto validChar : VALID_QUERY_CHARS) if (c == validChar) return;

    THROW (std::logic_error (STR+ "Malformed Query (invalid char detected): " + c + "[" + std::to_string ((short) c) + "])"));
}

inline bool Parser::isWordChar (char c) {
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
            isWord = isKeyWord = false;
        }
        if (c == '(') isKeyWord = true;
        tokenised += isKeyWord ? std::toupper (c) : c;
    }

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
        if (CLOSE_PAREN (c)) return;
        if (OPEN_PAREN (c)) {
            ParseTree * ptptr = tree->getInner();
            if (c == '(') {
                tree->type = Token::FUNCTION;
                if (!tree->getParentType()) ptptr->type = Token::DATABASE;
            } else if (c == '[') {
                tree->type = Token::LIST;
                ptptr->type = Token::VALUE;
            }
            else tree->type = Token::KV_PAIR;
            readToken (source, end, ptptr);
            continue;
        }
        if (SEPARATOR (c)) {
            ParseTree * ptptr = tree->getNext();
            if (c == '.') {
                char firstChar = tree->token [0];
                if (std::isalpha (firstChar)) tree->type = Token::DATABASE;
                if (firstChar == '@') tree->type = Token::USER;
                if (firstChar == '#') tree->type = Token::TABLE;
            } else if (c == ',') {
                if (!tree->type)  tree->type = Token::VALUE;
                if (!ptptr->type) ptptr->type = Token::VALUE;
            } else /*(c == ':')*/{
                tree->type = Token::KEY;
                ptptr->type = Token::VALUE;
            }
            readToken (source, end, ptptr);
            continue;
        }
        if (c == '@') tree->type = Token::USER;
        if (c == '#') tree->type = Token::TABLE;
        if (tree->type == Token::VALUE) {
            if (std::isdigit (c)) {
                if (!tree->token) tree->token += '$';
            }
        } else if (std::isdigit (c)) {
            THROW (std::logic_error (STR+
            "Numbers may only be used as value; found '" + c + "' within Token '" + tree->token + "' {" + std::to_string (tree->type) + "}"));
        } else {
            LOG_ASSERT (isWordChar (c));
        }
        tree->token += c;
    }
}

kj::Own <Query> Parser::buildQuery (kj::Own <ParseTree> const & pt) {
    kj::Own <Query> structure = kj::heap <Query>();
    ParseTree const * token = & * pt;

    // Set database name and action
    switch (token->type) {
        case Token::Type::DATABASE: {
            structure->database = token->getTokenName();
            structure->actionOnDatabase = Database::CHANGE;
            LOG_ASSERT (! token->tryGetInner());
            if (! token->tryGetNext()) {
                THROW (std::logic_error (STR +
                "Method on Database '" + structure->database + "' expected; found nothing"));
            }
            token = token->tryGetNext();
            switch (token->type) {
                case Token::Type::USER:
                case Token::Type::TABLE: {
                    structure->targetType = (token->type == Token::Type::USER) ?
                            Database::Target::USER :
                            Database::Target::TABLE;
                    structure->target = token->getTokenName();
                    structure->actionOnTarget = Database::Target::SELECT;
                    LOG_ASSERT (! token->tryGetInner());
                    if (!token->tryGetNext()) {
                        THROW (std::logic_error (STR +
                        "Method on Target '" + structure->target +
                        "' expected; found nothing"));
                    } token = token->tryGetNext();
                    if (!token->tryGetInner()) {
                        THROW (std::logic_error (STR +
                        "Parameters for function call on '" + structure->target +"' expected; found nothing"));
                    }
                    switch (structure->targetType) {
                        case Database::Target::TABLE: {
                            auto spec = Database::Target::Table::Specification();
                            spec.action = (Database::Target::Table::Action) lookUpEnum (
                                    token->getTokenName(), Database::Target::Table::ActionStrings);
                            fillInSpecs (token->tryGetInner(), spec);
                            structure->spec = std::move (Database::Target::Specification (spec));
                        } break;

                        case Database::Target::USER: {
                            auto spec = Database::Target::User::Specification();
                            spec.action = (Database::Target::User::Action) lookUpEnum (
                                    token->getTokenName(), Database::Target::User::ActionStrings);
                            fillInSpecs (token->tryGetInner(), spec);
                            structure->spec = std::move (Database::Target::Specification (spec));
                        } break;

                        default:
                            LOG_ASSERT (false);
                    }
                } break;

                case Token::Type::FUNCTION: {
                    structure->actionOnTarget = (Database::Target::Action)
                            lookUpEnum (token->getTokenName(), Database::Target::ActionStrings);
                    if (! token->tryGetInner()) {
                        THROW (std::logic_error (STR +
                        "Function call parameters for '" + token->getTokenName() + "' expected; nothing found"));
                    }
                    token = token->tryGetInner();
                    switch (token->type) {
                        case Token::Type::TABLE:
                        case Token::Type::USER: {
                            structure->targetType = (token->type == Token::Type::USER) ?
                                                    Database::Target::USER :
                                                    Database::Target::TABLE;
                            structure->target = token->getTokenName();
                        } break;

                        default:
                        THROW (std::logic_error (STR +
                        "Function call parameter for '" +
                        Database::Target::ActionStrings [structure->actionOnTarget] +
                        "' expected; Token found (" + token->getTokenName() + ", {" +
                        std::to_string (token->type) + "})"));
                    }
                    if (structure->actionOnTarget == Database::Target::Action::CREATE) {
                        if (structure->targetType == Database::Target::Type::TABLE) {
                            if (! (token = token->tryGetNext())) {
                                THROW (std::logic_error ("Expected List of column names; found nothing"));
                            }
                            ParseTree * tree = token->tryGetInner();
                            auto spec = Database::Target::Table::Specification();
                            do {
                                if (tree->type == Token::VALUE) {
                                    spec.values.emplace_back (tree->token, Cell());
                                } else {
                                    LOG (FATAL) << "Expected KV_PAIR or VALUE in List, found {" << tree->type << "}";
                                }
                            } while ((tree = tree->tryGetNext()));
                            structure->spec = std::move (Database::Target::Specification (spec));
                        }
                    }
                } break;

                default:
                    THROW (std::logic_error (STR +
                    "Expected Function, or Table or User; Token found (" + token->getTokenName() +
                    ", {" + std::to_string (token->type) + "})"));
            }
        } break;

        case Token::Type::FUNCTION: {
            structure->actionOnDatabase = (Database::Action) lookUpEnum (
                    token->getTokenName(), Database::ActionStrings);
            if (! token->tryGetInner()) {
                THROW (std::logic_error (STR +
                "Function call parameter for '" + token->getTokenName() + "' expected; nothing found"));
            }
            if (structure->actionOnDatabase == Database::Action::DATABASES ||
                structure->actionOnDatabase == Database::Action::USERS)
                return structure;
            token = token->tryGetInner();
            if (token->type == Token::DATABASE) structure->database = token->getTokenName();
            else {
                THROW (std::logic_error (STR +
                "Function call parameter for '" + Database::ActionStrings[structure->actionOnDatabase] +
                "' expected; function found (" + token->getTokenName() + ", {" + std::to_string (token->type) + "})"));
            }
            return structure;
        } break;

        default:
            THROW (std::logic_error (STR+ "Expected Function or Database;" +
            " Token found (" + token->getTokenName() + ", {" + std::to_string (token->type) + "})"));
    }
    return structure;
}

inline short Parser::lookUpEnum (std::string const & str, std::vector <std::string> const & enums) {
    short index = 0;
    for (auto const & e : enums) {
        if (str == e) return index;
        index++;
    }
    THROW (std::logic_error (STR+ "Invalid Keyword found: '" + str + "'"));
}

void Parser::fillInSpecs (ParseTree const * tree, Database::Target::Table::Specification & specs) {
    if (!tree) LOG (FATAL) << "Missing parameters detected for filling in specifications for " <<
            Database::Target::Table::ActionStrings [specs.action];

    switch (specs.action) {
        case Database::Target::Table::READ: {
            if (! (tree->tryGetInner() && tree->tryGetNext() && tree->tryGetNext()->tryGetInner())) {
                THROW (std::logic_error (STR+
                "Expected a list [COLUMN] and a list of maps [{COLUMN : value}]; found: '" + tree->str() + "'"));
            }
            if (! (tree->type == Token::LIST && tree->tryGetNext()->type == Token::LIST)) {
                THROW (std::logic_error (STR+
                                         "For SELECT function, expected a list [COLUMN] " +
                                         "and a list of maps [{COLUMN : value}] of parameters"));
            }
            fillPairLists (tree, specs.values);
            fillPairLists (tree, specs.where);
        } break;

        case Database::Target::Table::INSERT: {
            if (! (tree->tryGetInner())) {
                if (! (tree->type == Token::LIST)) {
                    THROW (std::logic_error (
                            "For INSERT function, expected a list of maps [{COLUMN : value}] as parameter"));
                }
            }
            fillPairLists (tree, specs.where);
        } break;

        case Database::Target::Table::REMOVE: {
            if (! (tree->tryGetInner())) {
                if (! (tree->type == Token::LIST)) {
                    THROW (std::logic_error (
                            "For REMOVE function, expected a list of maps [{COLUMN : value}] as parameter"));
                }
            }
            fillPairLists (tree, specs.where);
        } break;

        case Database::Target::Table::UPDATE: {
            if (! (tree->tryGetInner() && tree->tryGetNext() && tree->tryGetNext()->tryGetInner())) {
                if (! (tree->type == Token::LIST && tree->tryGetNext()->type == Token::LIST)) {
                    THROW (std::logic_error (STR +
                                             "For SELECT function, expected a list of maps [{COLUMN : value}] each as values and spec parameters"));
                }
            }
            fillPairLists (tree, specs.values);
            fillPairLists (tree, specs.where);
        } break;

        default:
            LOG (FATAL) << "Invalid action detected";
    }
}
void Parser::fillInSpecs (ParseTree const * tree, Database::Target::User::Specification & specs) {}

void Parser::fillPairLists (ParseTree const * tree, std::vector <std::pair <std::string, Cell>> & list) {
    if (tree->type != Token::LIST || !tree->tryGetInner()) {
        THROW (std::logic_error ("List not found"));
    }
    tree = tree->tryGetInner();
    do {
        if (tree->type == Token::KV_PAIR) {
            ParseTree * key = tree->tryGetInner();
            ParseTree * val = key->tryGetNext();
            if (key->type == Token::KEY && val && val->type == Token::VALUE) {
                list.emplace_back (key->token, tokenToCell (val->token));
            }
        } else if (tree->type == Token::VALUE) {
            list.emplace_back (tree->token, Cell());
        } else {
            LOG (FATAL) << "Expected KV_PAIR or VALUE in List, found {" << tree->type << "}";
        }
    } while ((tree = tree->tryGetNext()));
}
Cell Parser::tokenToCell (std::string const & token) {
    if (token.empty()) return Cell();
    if (token [0] != '$') return Cell (token);
    return strToNum (token);
}

inline Cell Parser::strToNum (std::string const & token) {
    if (token.size() == 2) return Cell ((bool) (token [1] - '0'));
    long sum = 0;
    long factor = 1;
    for (std::size_t idx = token.size(); --idx > 0;) {
        sum += (token [idx] - '0') * factor;
        factor *= 10;
    }
    if (sum > -1 * (long) pow (2, 8 * sizeof (short)) && sum < (long) pow (2, 8 * sizeof (short)) - 1) {
        return Cell (short (sum));
    } return Cell (sum);
}
/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */