#include "Wrapper.h"

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrap (Table const & table) {
    kj::Own <capnp::MallocMessageBuilder> responseBuilder = kj::heap <capnp::MallocMessageBuilder>();

    RPCServer::Table::Builder builder = responseBuilder->initRoot <::RPCServer::Table>();
    auto metaBuilder = builder.initMeta (table.columnCount ());
    std::vector <Meta> meta = table.getMeta();
    std::size_t idx = 0;
    for (auto const & col : meta) {
        metaBuilder.setWithCaveats (idx++, wrap (col)->getRoot <RPCServer::Table::Meta>().asReader());
    }

    auto contentBuilder = builder.initContent (table.rowCount ());
    for (std::size_t r = 0; r < table.rowCount (); r++) {
        auto rowBuilder = contentBuilder [r].initData (table.columnCount ());
        std::size_t c = 0;
        for (auto const & cell : table.readRowAsVector (r)) {
            auto cellBuilder = wrap (cell);
            rowBuilder.setWithCaveats (c++, cellBuilder->getRoot <RPCServer::Table::Cell>());
            LOG_ASSERT (cell == unwrap (rowBuilder.asReader()[c-1]));
        }
    }

    return responseBuilder;
}

kj::Own <Table> Wrapper::unwrap (::RPCServer::Table::Reader const & reader) {
    std::vector <Meta> metae;
    for (auto const & meta : reader.getMeta ()) metae.push_back (unwrap (meta));
    kj::Own <Table> table = kj::heap <Table> (metae, "", "");
    //TODO: Serialise Database- and Tablename

    for (auto const & row : reader.getContent ()) {
        std::vector <Cell> newRow;
        for (auto const & wrappedCell : row.getData()) newRow.push_back (unwrap (wrappedCell));
        table->createRow (newRow);
    }
    return table;
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrap (Meta const & meta) {
    kj::Own <capnp::MallocMessageBuilder> metaBuilder = kj::heap <capnp::MallocMessageBuilder>();
    RPCServer::Table::Meta::Builder builder = metaBuilder->initRoot <RPCServer::Table::Meta>();
    builder.setName (meta.name);
    builder.setDataType (meta.dataType);
    builder.setKeyType (meta.keyType);
    builder.setIndex (meta.index);
    builder.setNullable (meta.nullable);

    auto dataBuilder = builder.initReference();
    meta.reference.index() ?
        dataBuilder.setValue(std::get <std::string> (meta.reference)) :
        dataBuilder.setNothing();

    return metaBuilder;
}

Meta Wrapper::unwrap (RPCServer::Table::Meta::Reader const & reader) {
    Meta meta;
    meta.name     = reader.getName ();
    meta.dataType = (CellType) reader.getDataType();
    meta.keyType  = (KeyType)  reader.getKeyType ();
    meta.index    = reader.getIndex();
    meta.nullable = reader.getNullable();

    auto ref = reader.getReference ();
    if (ref.hasValue()) meta.reference = Key::Reference (ref.getValue());

    return std::move (meta);
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrap (Cell const & cell) {
    kj::Own <capnp::MallocMessageBuilder> cellBuilder = kj::heap <capnp::MallocMessageBuilder> ();
    RPCServer::Table::Cell::Builder builder = cellBuilder->initRoot <::RPCServer::Table::Cell>();

    switch (cell.index ()) {
        case UNARY:
            LOG (INFO) << "Set Cell to Unary";
            builder.setUnary();
            break;
        case BINARY:
            LOG (INFO) << "Set Cell to Binary: " << std::get <bool> (cell);
            builder.setBinary (std::get <bool> (cell));
            break;
        case SHORT:
            LOG (INFO) << "Set Cell to Short: " << std::get <short> (cell);
            builder.setShort (std::get <short> (cell));
            break;
        case LONG:
            LOG (INFO) << "Set Cell to Long: " << std::get <long> (cell);
            builder.setLong (std::get <long> (cell));
            break;
        case TEXT:
            LOG (INFO) << "Set Cell to Text: " << std::get <std::string> (cell);
            builder.setText (std::get <std::string> (cell));
            break;
        default:
            LOG (FATAL) << "Cell Wrapper went insane: " << cell.index() << " not in range [0, 5)";
    }
    LOG_ASSERT (cell.index() == builder.asReader().which());
    LOG_ASSERT (cell.index() == cellBuilder->getRoot <RPCServer::Table::Cell>().which());

    return cellBuilder;
}

Cell Wrapper::unwrap (RPCServer::Table::Cell::Reader const & cell) {
    switch (cell.which()) {
        case RPCServer::Table::Cell::UNARY:
            return Cell();
        case RPCServer::Table::Cell::BINARY:
            return Cell (cell.getBinary());
        case RPCServer::Table::Cell::SHORT:
            return Cell ((short) cell.getShort());
        case RPCServer::Table::Cell::LONG:
            return Cell ((long) cell.getLong());
        case RPCServer::Table::Cell::TEXT:
            return Cell (cell.getText());
        default:
            LOG (FATAL) << "Cell Unwrapper went insane: " << (short) cell.which() << " not in range [0, 5)";
    }
}

kj::Own <capnp::MallocMessageBuilder> Wrapper::wrap (Target const & target) {
    kj::Own <capnp::MallocMessageBuilder> targetBuilder = kj::heap <capnp::MallocMessageBuilder> ();
    RPCServer::Target::Builder builder = targetBuilder->initRoot <::RPCServer::Target>();

    std::visit (TargetVisitor {builder}, target);

    LOG_ASSERT (target.index() == builder.asReader().which());
    LOG_ASSERT (target.index() == targetBuilder->getRoot <RPCServer::Target>().which());

    return targetBuilder;
}

Target Wrapper::unwrap (::RPCServer::Target::Reader const & reader) {
    switch (reader.which()) {
        case ::RPCServer::Target::DATABASE:
            return unwrap (reader.getDatabase());
        case ::RPCServer::Target::TABLE:
            return unwrap (reader.getTable());
        case ::RPCServer::Target::COLUMN:
            return unwrap (reader.getColumn());
        case ::RPCServer::Target::ROW:
            return unwrap (reader.getRow());
    }
}

qy::Database Wrapper::unwrap (::RPCServer::Target::Database::Reader const & reader) {
    return qy::Database (reader.getName());
}
qy::Table Wrapper::unwrap (::RPCServer::Target::Table::Reader const & reader) {
    return qy::Table (unwrap (reader.getParent()), reader.getName());
}
qy::Column Wrapper::unwrap (::RPCServer::Target::Column::Reader const & reader) {
    return qy::Column (unwrap (reader.getParent()), reader.getName());
}
qy::Row Wrapper::unwrap (::RPCServer::Target::Row::Reader const & reader) {
    return qy::Row (unwrap (reader.getParent()),
                    unwrap (reader.getColumns()),
                    unwrap (reader.getSpecs()));
}
qy::Specification Wrapper::unwrap (::RPCServer::Target::Row::Specification::Reader const & reader) {
    qy::Predicate pred;
    Cell cell;
    switch (reader.which()) {
        case RPCServer::Target::Row::Specification::EQUALS:
            pred = qy::EQUALS;
            cell = unwrap (reader.getEquals());
            break;
        case RPCServer::Target::Row::Specification::UNEQUALS:
            pred = qy::UNEQUALS;
            cell = unwrap (reader.getUnequals());
            break;
        case RPCServer::Target::Row::Specification::SMALLER:
            pred = qy::SMALLER;
            cell = unwrap (reader.getSmaller());
            break;
        case RPCServer::Target::Row::Specification::BIGGER:
            pred = qy::BIGGER;
            cell = unwrap (reader.getBigger());
            break;
    }
    return qy::Specification (unwrap (reader.getColumn()), cell, pred);
}
std::vector <qy::Column> Wrapper::unwrap (capnp::List <::RPCServer::Target::Column>::Reader const & reader) {
//    std::vector <qy::Column> columns (reader.size());
//    for (auto const & col : reader)
//        columns.push_back (unwrap (col));
//    return columns;
}
std::vector <qy::Specification> Wrapper::unwrap (capnp::List <::RPCServer::Target::Row::Specification>::Reader const & reader) {
//    std::vector <qy::Specification> specs (reader.size());
//    for (auto const & spec : reader)
//        specs.push_back (unwrap (spec));
//    return specs;
}
std::vector <Cell> Wrapper::unwrap (capnp::List <::RPCServer::Table::Cell>::Reader const & reader) {
//    std::vector <Cell> cells (reader.size());
//    for (auto const & cell : reader)
//        cells.push_back (unwrap (cell));
//    return cells;
}

Wrapper::TargetVisitor::TargetVisitor (RPCServer::Target::Builder & builder) : builder {builder} {}

void Wrapper::TargetVisitor::operator ()(qy::Database const & target) {
    init (builder.initDatabase(), target);
}
void Wrapper::TargetVisitor::operator ()(qy::Table const & target) {
    init (builder.initTable(), target);
}
void Wrapper::TargetVisitor::operator ()(qy::Column const & target) {
    init (builder.initColumn(), target);
}
void Wrapper::TargetVisitor::operator ()(qy::Row const & target) {
    init (builder.initRow(), target);
}

void Wrapper::init (RPCServer::Target::Database::Builder builder, qy::Database const & target) {
    builder.setName (target.name);
}
void Wrapper::init (RPCServer::Target::Table::Builder builder, qy::Table const & target) {
    builder.setName (target.name);
    init (builder.initParent(), target.parent);
}
RPCServer::Target::Column::Reader Wrapper::init (RPCServer::Target::Column::Builder builder, qy::Column const & target) {
    builder.setName (target.name);
    init (builder.initParent(), target.parent);
    return builder.asReader();
}
void Wrapper::init (RPCServer::Target::Row::Builder builder, qy::Row const & target) {
    init (builder.initParent(), target.parent);

    auto colBuilder = builder.initColumns (target.columns.size());
    std::size_t idx = 0;
    for (auto const & col : target.columns) {
        auto tmpBuilder = kj::heap <capnp::MallocMessageBuilder>()->initRoot<RPCServer::Target::Column>();
        colBuilder.setWithCaveats (idx++, init (tmpBuilder, col));
    }

    auto specBuilder = builder.initSpecs (target.specs.size());
    idx = 0;
    for (auto const & spec : target.specs) {
        auto tmpBuilder = kj::heap <capnp::MallocMessageBuilder>()->initRoot<RPCServer::Target::Row::Specification>();
        specBuilder.setWithCaveats (idx++, init (tmpBuilder, spec));
    }
}
RPCServer::Target::Row::Specification::Reader Wrapper::init (RPCServer::Target::Row::Specification::Builder builder, qy::Specification const & spec) {
    init (builder.initColumn(), spec.column);
    auto value = wrap (spec.value)->initRoot <::RPCServer::Table::Cell>().asReader();
    switch (spec.predicate_e) {
        case qy::Predicate::EQUALS:
            builder.setEquals (value);
            break;
        case qy::Predicate::UNEQUALS:
            builder.setUnequals (value);
            break;
        case qy::Predicate::SMALLER:
            builder.setSmaller (value);
            break;
        case qy::Predicate::BIGGER:
            builder.setBigger (value);
            break;
        default:
            LOG (FATAL) << "Specs Wrapper went insane: " << spec.predicate_e << " not in range [0, 4)";
    }
    return builder.asReader();
}


/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */