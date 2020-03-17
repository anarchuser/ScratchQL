#include "Query.h"

bool Database::Target::Table::Specification::operator == (Database::Target::Table::Specification const & other) const {
    return
            this->action == other.action &&
            this->row    == other.row    &&
            this->column == other.column &&
    true;
}

bool Database::Target::User::Specification::operator == (Database::Target::User::Specification const & other) const {
    return
            this->action == other.action &&
            this->target == other.target &&
    true;
}

bool Query::operator == (Query const & other) const {
    return
            this->actionOnDatabase == other.actionOnDatabase &&
            this->database         == other.database         &&

            this->targetType       == other.targetType       &&
            this->actionOnTarget   == other.actionOnTarget   &&
            this->target           == other.target           &&

            this->spec             == other.spec             &&
            this->data             == other.data             &&
    true;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */