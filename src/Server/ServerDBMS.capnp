@0xeae6fc23a157ce01;

interface RPCServer {
    struct Table {
        header @0 :List(Text);
        meta @1 :List(UInt8);
        content @2 :List(Row);

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

    sendQuery @0 (query :Text) -> (table :Table);
    connect @1 () -> ();
}
