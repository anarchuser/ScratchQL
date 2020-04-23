#include "../../../main.h"
#include "../../../../src/DBMS/Table/Index/Index.h"

#include <string>
#include <algorithm>

#define SEED 1
#define N_INSERTS   1000
#define MAX_STR_LEN 10

SCENARIO("I can create indices for large amounts of data") {

    auto rand_str = [] () -> std::string {
        std::string str;
        int length = std::rand() % MAX_STR_LEN + 1;
        for (int i = 0; i < length; i++) str += (char) std::rand();
        return str;
    };

    std::srand (SEED);

    GIVEN ("A list of bools") {
        const std::size_t VEC_SIZE = N_INSERTS;
        std::vector <bool> bools (VEC_SIZE);
        for (std::size_t i = 0; i < VEC_SIZE; i++) {
            bools.push_back (std::rand() % 2);
        }
        WHEN ("I store them in an Index") {
            Index index (CellType::BINARY, false);
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                CHECK (index.insert (bools [i], i));
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    bool contains = false;
                    idx::Rows rows = index.select (bools [i]);
                    for (auto const & val : std::get <std::vector <std::size_t>> (rows)) {
                        contains = contains || val == i;
                    }
                    CHECK (contains);
                }
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (index.remove (bools [i], i));
                }
            }
        }
    }
    GIVEN ("A list of strings ") {
        const std::size_t VEC_SIZE = N_INSERTS;
        std::vector <std::string> strings;
        WHEN ("I initialise them and store them in a non-unique Index") {
            Index normalIDX (CellType::TEXT, false);
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                strings.push_back (rand_str());
                CHECK (normalIDX.insert (strings [i], i));
                CHECK (normalIDX.insert (strings [i], i));
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (normalIDX.select (strings [i]).index() == 2);
                    CHECK (!std::get <std::vector <std::size_t>> (normalIDX.select (strings [i])).empty());
                }
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (normalIDX.remove (strings [i], i));
                }
            }
        }
        WHEN ("I initialise them and store them in a unique Index") {
            Index uniqueIDX (CellType::TEXT, true);
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                std::string str = rand_str();
                if (!str) continue;
                if (std::find (strings.begin(), strings.end(), str) != strings.end()) {
                    --i;
                    continue;
                }
                else strings.push_back (str);
                CHECK ( uniqueIDX.insert (strings [i], i));
                CHECK (!uniqueIDX.insert (strings [i], i));
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (uniqueIDX.select (strings [i]).index() == 1);
                    CHECK (std::get <std::size_t> (uniqueIDX.select (strings [i])) == i);
                }
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (uniqueIDX.remove (strings [i], i));
                }
            }
        }
    }
//    GIVEN ("A list of shorts") {
//        const std::size_t VEC_SIZE = N_INSERTS;
//        std::vector <short> shorts (VEC_SIZE);
//        Index index (CellType::SHORT, false);
//        for (std::size_t i = 0; i < VEC_SIZE; i++) {
//            shorts.push_back (std::rand());
//        }
//        WHEN ("I store them in an Index") {
//            for (std::size_t i = 0; i < VEC_SIZE; i++) {
//                CHECK (index.insert (shorts.back(), i));
//            }
//            THEN ("I can retrieve their values from the Index") {
//                for (std::size_t i = 0; i < VEC_SIZE; i++) {
//                    bool contains = false;
//                    idx::Rows rows = index.select (shorts [i]);
//                    for (auto const & val : std::get <std::vector <std::size_t>> (rows)) {
//                        contains = contains || val == i;
//                    }
//                    CHECK (contains);
//                }
//            }
//            THEN ("I can remove entries from the Index, given a cell and its respective row") {
//                for (std::size_t i = 0; i < VEC_SIZE; i++) {
//                    CHECK (index.remove (shorts [i], i));
//                }
//            }
//        }
//    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */