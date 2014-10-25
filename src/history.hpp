#ifndef HISTORY_HPP
#define HISTORY_HPP

#include "colors.hpp"
#include "coords.hpp"
#include "moves.hpp"

namespace History {
    extern UNumspeed table[1ull << Coord::usedBits][1ull << Coord::usedBits];

    inline UNumspeed score(Move::Type move) {
        Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
        Coord::Type to   = move & Coord::typeMask;
        
        return table[from][to];
    }

    inline void alphed(Move::Type move, UNumspeed depth) {
        Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
        Coord::Type to   = move & Coord::typeMask;

        table[from][to] += 1 << depth;
    }

    inline void beted(Move::Type move, UNumspeed depth) {
        Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
        Coord::Type to   = move & Coord::typeMask;

        table[from][to] += 1 << depth;
    }

    inline void flush() {
        for(Coord::Type from = 0; from < 64; ++from) {
            for(Coord::Type to = 0; to < 64; ++to) {
                table[from][to] = 0;
            }
        }
    }

    inline void split() {
        for(Coord::Type from = 0; from < 64; ++from) {
            for(Coord::Type to = 0; to < 64; ++to) {
                table[from][to] /= 100;
            }
        }
    }
}

#endif /* HISTORY_HPP */
