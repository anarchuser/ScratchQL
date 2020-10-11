#include "../../../main.h"
#include "../../../../src/DBMS/Table/Index/Index.h"

#include <string>
#include <algorithm>

#define SEED                3
#define N_INSERTS           200
#define N_UNIQUE_INSERTS    200
#define MAX_STR_LEN         16

SCENARIO("I can create indices for large amounts of data") {
    SECTION ("Constructor and Destructor work") {
        CHECK_NOTHROW (delete new Index (CellType::LONG, false));
    }

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
                CHECK (index.insert ((bool) bools [i], i));
            }
            WHEN ("I store the values to file") {
                auto PATH = std::filesystem::path ("tmp/indices/bools.idx");
                CHECK_NOTHROW (index.save (PATH));
                THEN ("I can retrieve them again") {
                    CHECK_NOTHROW (Index (PATH).dump());
//                    CHECK (index.dump() == Index (PATH).dump());
                }
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    bool contains = false;
                    idx::Rows rows = index.select ((bool) bools [i]);
                    for (auto const & val : std::get <std::vector <std::size_t>> (rows)) {
                        contains = contains || val == i;
                    }
                    CHECK (contains);
                }
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (index.remove ((bool) bools [i], i));
                }
            }
        }
    }
    GIVEN ("A list of strings ") {
        std::vector <std::string> strings;
        WHEN ("I initialise them and store them in a non-unique Index") {
            const std::size_t VEC_SIZE = N_INSERTS;
            Index index (CellType::TEXT, false);
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                strings.push_back (rand_str());
                CHECK (  index.insert (strings [i], i));
                CHECK (! index.insert (strings [i], i));
                CHECK (  index.insert (strings [i], i + VEC_SIZE));
            }
            WHEN ("I store the values to file") {
                auto PATH = std::filesystem::path ("tmp/indices/strings_normal.idx");
                CHECK_NOTHROW (index.save (PATH));
                THEN ("I can retrieve them again") {
                    CHECK_NOTHROW (Index (PATH).dump());
//                    CHECK (index.dump() == Index (PATH).dump());
                }
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (index.select (strings [i]).index() == 2);
                    CHECK (!std::get <std::vector <std::size_t>> (index.select (strings [i])).empty());
                }
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (index.remove (strings [i], i));
                }
            }
        }
        WHEN ("I initialise them and store them in a unique Index") {
            const std::size_t VEC_SIZE = N_UNIQUE_INSERTS;
            Index index (CellType::TEXT, true);
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                std::string str = rand_str();
                if (!str) continue;
                if (std::find (strings.begin(), strings.end(), str) != strings.end()) { --i; continue; }
                else strings.push_back (str);
                CHECK (index.insert (strings [i], i));
                CHECK (!index.insert (strings [i], i));
            }
            WHEN ("I store the values to file") {
                auto PATH = std::filesystem::path ("tmp/indices/strings_unique.idx");
                CHECK_NOTHROW (index.save (PATH));
                THEN ("I can retrieve them again") {
                    CHECK_NOTHROW (Index (PATH).dump());
//                    CHECK (index.dump() == Index (PATH).dump());
                }
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (index.select (strings [i]).index() == 1);
                    CHECK (std::get <std::size_t> (index.select (strings [i])) == i);
                }
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (index.remove (strings [i], i));
                }
            }
        }
    }
    GIVEN ("A list of shorts") {
        std::vector <short> shorts;
        std::vector <short> unique;
        WHEN ("I store them in a normal Index") {
            const std::size_t VEC_SIZE = N_INSERTS;
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                short rand = std::rand();
                if (std::find (shorts.begin(), shorts.end(), rand) != shorts.end()) --i;
                else shorts.push_back (std::rand());
            }
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                short rand = std::rand();
                if (std::find (shorts.begin(), shorts.end(), rand) != shorts.end()) --i;
                else unique.push_back (rand);
            }
            Index index (CellType::SHORT, false);
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                CHECK ( index.insert (shorts [i], i));
                CHECK (!index.insert (shorts [i], i));
                CHECK ( index.insert (shorts [i], i + VEC_SIZE));
            }
            WHEN ("I store the values to file") {
                auto PATH = std::filesystem::path ("tmp/indices/shorts_normal.idx");
                CHECK_NOTHROW (index.save (PATH));
                THEN ("I can retrieve them again") {
                    CHECK_NOTHROW (Index (PATH).dump());
                    CHECK (index.dump() == Index (PATH).dump());
                }
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    bool contains = false;
                    idx::Rows rows;
                    CHECK_NOTHROW (rows = index.select (shorts [i]));
                    for (auto const & val : std::get <std::vector <std::size_t>> (rows)) {
                        contains = contains || val == i;
                    }
                    CHECK (contains);
                }
            }
            THEN ("Trying to retrieve non-existent values fails") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (!index.select (unique [i]).index());
                }
            }
            THEN ("I can print it to an ostream") {
                std::stringstream is;
                CHECK_NOTHROW (is << index.str ());
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK ( index.remove (shorts [i], i));
                    CHECK (!index.remove (shorts [i], i));

                    CHECK ( index.remove (shorts [i], i + VEC_SIZE));
                    CHECK (!index.remove (shorts [i], i + VEC_SIZE));
                }
            }
        }
        WHEN ("I store them in a unique Index") {
            const std::size_t VEC_SIZE = N_UNIQUE_INSERTS;
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                short rand = std::rand();
                if (std::find (shorts.begin(), shorts.end(), rand) != shorts.end()) --i;
                else shorts.push_back (std::rand());
            }
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                short rand = std::rand();
                if (std::find (shorts.begin(), shorts.end(), rand) != shorts.end()) --i;
                else unique.push_back (rand);
            }
            Index index (CellType::SHORT, true);
            for (std::size_t i = 0; i < VEC_SIZE; i++) {
                CHECK ( index.insert (shorts [i], i));
                CHECK (!index.insert (shorts [i], i));
            }
            WHEN ("I store the values to file") {
                auto PATH = std::filesystem::path ("tmp/indices/shorts_unique.idx");
                CHECK_NOTHROW (index.save (PATH));
                THEN ("I can retrieve them again") {
                    CHECK_NOTHROW (Index (PATH).dump());
                    CHECK (index.dump() == Index (PATH).dump());
                }
            }
            THEN ("I can retrieve their values from the Index") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    REQUIRE (index.select (shorts [i]).index() == 1);
                    CHECK (std::get <std::size_t> (index.select (shorts [i])) == i);
                }
            }
            THEN ("I can print and dump it to an ostream") {
                std::stringstream is;
                CHECK_NOTHROW (is << index.str ());
                CHECK_NOTHROW (index.dump());
            }
            THEN ("Trying to retrieve non-existent values fails") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK (!index.select (unique [i]).index());
                }
            }
            THEN ("I can remove entries from the Index, given a cell and its respective row") {
                for (std::size_t i = 0; i < VEC_SIZE; i++) {
                    CHECK ( index.remove (shorts [i], i));
                    CHECK (!index.remove (shorts [i], i));
                }
            }
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */