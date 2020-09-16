@0xeae6fc23a157ce01;

interface RPCServer {
    struct Table {
        meta @0 :List(Meta);
        content @1 :List(Row);

        struct Meta {
            name @0 :Text;
            dataType @1 :UInt32;
            keyType @2 :UInt32;
            reference @3 :Maybe(Text;
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

    struct Maybe(T) {
        union {
            empty @0 :Void;
            value @1 :T;
        }
    }

    sendQuery @0 (query :Text) -> (response :Maybe(Table));
    connect @1 () -> ();
}
