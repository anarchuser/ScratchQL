@0xeae6fc23a157ce01;

interface Database {
    struct TableE {
        header @0: List(Text);
        content @1: List(List(CellE));

        struct CellE {
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

    sendQuery @0 (query :Text) -> (table :TableE);
}
