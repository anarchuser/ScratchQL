@0xeae6fc23a157ce01;

interface RPCServer {
    struct Table {
        meta @0 :List(Meta);
        content @1 :List(Row);

        struct Meta {
            name @0 :Text;
            dataType @1 :UInt32;
            keyType @2 :UInt32;
            reference @3 :Reference;
            index @4 :Bool;
            nullable @5 :Bool;
        }

        struct Reference {
            data :union {
                unary @0 :Void;
                table @1 :Text;
            }
        }

        struct Row {
            data @0 :List(Cell);
        }

        struct Cell {
            data :union {
                unary @0 :Void;
                binary @1 :Bool;
                short @2 :UInt32;
                long @3 :UInt64;
                text @4 :Text;
                # Corresponds to a std::variant
            }
        }
    }

    struct Response {
        data :union {
            void @0 :Void;
            table @1 :Table;
        }
    }

    sendQuery @0 (query :Text) -> (response :Response);
    connect @1 () -> ();
}
