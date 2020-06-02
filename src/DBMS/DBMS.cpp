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
    return Response();
}

Response DBMS::evalTableQuery (kj::Own <Query> const & query) {
    if (query->actionOnTarget == Database::Target::Action::CREATE) {
        auto const & spec = std::get <Database::Target::Table::Specification> (query->spec);
        std::vector <Meta> header;
        for (auto const & pair : spec.values) {
            header.emplace_back (pair.first, TEXT, NORMAL, false, true);
        }
        auto table = kj::heap <Table> (header);
        table->initDiskMode (query->database, query->target);
        return table;
    }

    /* TEST IMPLEMENTATION. REMOVE AFTER SUCCESSFUL QUERY EXECUTION */
    std::vector <Meta> header {
            {"surname", TEXT, PRIMARY, true, false},
            {"name", TEXT, PRIMARY, true, false},
            {"age", SHORT, NORMAL, true, false},
            {"profession", TEXT, "Professions", false, true},
    };
    auto testTable = kj::heap <Table> (header);
    testTable->createRow(std::vector <Cell> {std::string ("Adam"), std::string ("Abcd"), short (30), Cell()});
    testTable->createRow(std::vector <Cell> {std::string ("Tom"),  std::string ("Efgh"), short (30), std::string ("jfuesfeoies")});
    testTable->createRow(std::vector <Cell> {std::string ("Eve"),  std::string ("Ijkl"), short (30), std::string ("nfwiufew")});
    testTable->createRow(std::vector <Cell> {std::string ("Dora"), std::string ("Mnop"), short (30), std::string ("jfuesfeoies")});

    return testTable;
}

Response DBMS::evalUserQuery (kj::Own <Query> const & query) {
    return kj::heap <Table> (std::vector <Meta> {
        {"USER", TEXT, "_Users", false, false},
        {"PERMISSION", TEXT, "_Permissions", false, false}
    });
}

std::ostream & operator << (std::ostream & os, Response const & response) {
    switch (response.index()) {
        case ResponseType::VOID:
            return os;
        case ResponseType::TABLE:
            return os << * std::get <kj::Own <Table const>> (response);
        default:
            LOG (FATAL) << "Response has gone insane";
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */