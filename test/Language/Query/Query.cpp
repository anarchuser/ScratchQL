#include "../../main.h"
#include "../../../src/Language/Query/Query.h"

TEST_CASE ("Queries can be built and compared to each other") {
    GIVEN ("Some Specifications for tables or users") {
        Database::Target::Table::Specification tspec;
        Database::Target::User::Specification uspec;

        Database::Target::Specification varvspec;
        Database::Target::Specification vartspec = Database::Target::Specification (tspec);
        Database::Target::Specification varuspec = Database::Target::Specification (uspec);

        CHECK (varvspec.index() == (int) Database::Target::VOID);
        CHECK (vartspec.index() == (int) Database::Target::TABLE);
        CHECK (varuspec.index() == (int) Database::Target::USER);

        CHECK (tspec == std::get <Database::Target::Table::Specification> (vartspec));
        CHECK (uspec == std::get <Database::Target::User::Specification>  (varuspec));

        tspec.action = Database::Target::Table::INSERT;
        uspec.action = Database::Target::User::GIVE;
        CHECK (tspec != std::get <Database::Target::Table::Specification> (vartspec));
        CHECK (uspec != std::get <Database::Target::User::Specification>  (varuspec));
    }

    GIVEN ("Some Queries") {
        Query first, second;

        first.actionOnDatabase  = Database::CREATE;
        second.actionOnDatabase = Database::CHANGE;

        first.database  = "foo";
        second.database = "bar";

        second.targetType = Database::Target::USER;
        second.actionOnTarget = Database::Target::CREATE;
        second.target = "test";

        CHECK (first == first);
        CHECK (second == second);
        CHECK (!(first == second));
        CHECK (!(second == first));
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */