#include "DBMS.h"

Response DBMS::evalQuery (std::string const & rawQuery) {
    kj::Own <Query> procQuery = Parser::parseQuery (rawQuery);
    switch (procQuery->targetType) {
        case Database::Target::Type::VOID:
            break;
        case Database::Target::Type::TABLE:
            return evalTableQuery (procQuery);
        case Database::Target::Type::USER:
            return evalUserQuery (procQuery);
        default:
            LOG (FATAL) << "Invalid Target Type - Expected [0, 2], got " << procQuery->targetType;
    }
    return kj::heap <Table const> (std::vector <std::string>({"a"}));
}

Response DBMS::evalTableQuery (kj::Own <Query> const & query) {
    if (query->actionOnTarget == Database::Target::Action::CREATE) {
        auto const & spec = std::get <Database::Target::Table::Specification> (query->spec);
        std::vector <std::string> header;
        for (auto const & pair : spec.values) header.emplace_back (pair.first);
        return kj::heap <Table const> (header);
    }

    /* TEST IMPLEMENTATION. REMOVE AFTER SUCCESSFUL QUERY EXECUTION */
    auto testTable = kj::heap <Table> (std::vector <std::string> {"surname", "name", "age", "profession"});
    testTable->createRow(std::vector <Cell> {std::string ("Adam"), std::string ("Abcd"), short (30), Cell()});
    testTable->createRow(std::vector <Cell> {std::string ("Tom"),  std::string ("Efgh"), short (30), std::string ("jfuesfeoies")});
    testTable->createRow(std::vector <Cell> {std::string ("Eve"),  std::string ("Ijkl"), short (30), std::string ("nfwiufew")});
    testTable->createRow(std::vector <Cell> {std::string ("Dora"), std::string ("Mnop"), short (30), std::string ("jfuesfeoies")});

    return testTable;
}

Response DBMS::evalUserQuery (kj::Own <Query> const & query) {
    return kj::heap <Table> (std::vector <std::string> { "USER", "PERMISSION" });
}

std::ostream & operator << (std::ostream & os, Response const & response) {
    switch (response.index()) {
        case ResponseType::VOID:
            break;
        case ResponseType::TABLE:
            os << * std::get <kj::Own <Table const>> (response);
            break;
        default:
            LOG (FATAL) << "Response has gone insane";
    }
    return os;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */