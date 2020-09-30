@0xeae6fc23a157ce01;

interface RPCServer {
    struct Table {
        meta @0 :List(Meta);
        content @1 :List(Row);

        struct Meta {
            name @0 :Text;
            dataType @1 :UInt32;
            keyType @2 :UInt32;
            reference @3 :Maybe(Text);
            index @4 :Bool;
            nullable @5 :Bool;
        }

        struct Row {
            data @0 :List(Cell);
        }

        struct Cell {
            union {
                unary @0 :Void;
                binary @1 :Bool;
                short @2 :UInt32;
                long @3 :UInt64;
                text @4 :Text;
                # Corresponds to a std::variant
            }
        }
    }

    struct Target {
        union {
            database @0 :Database;
            table @1 :Table;
            column @2 :Column;
            row @3 :Row;
        }

        struct Database {
            name @0 :Text;
        }
        struct Table {
            name @0 :Text;
            parent @1 :Database;
            metae @2 :Maybe(List(RPCServer.Table.Meta));
        }
        struct Column {
            name @0 :Text;
            parent @1 :Table;
            meta @2 :Maybe(RPCServer.Table.Meta);
        }
        struct Row {
            parent @0 :Table;
            columns @1 :List(Column);
            data @2 :List(RPCServer.Table.Cell);
            specs @3 :List(Specification);

            struct Specification {
                column @0 :Column;
                union {
                    equals @1 :RPCServer.Table.Cell;
                    unequals @2 :RPCServer.Table.Cell;
                    smaller @3 :RPCServer.Table.Cell;
                    bigger @4 :RPCServer.Table.Cell;
                }
            }
        }
    }

    struct Maybe(T) {
        union {
            nothing @0 :Void;
            value @1 :T;
       }
    }

    connect @0 () -> ();
    create @1 (target :Target) -> ();
    select @2 (target :Target) -> (data :Table);
    modify @3 (target :Target.Row) -> ();
    insert @4 (target :Target.Row) -> ();
    remove @5 (target :Target) -> ();
}
