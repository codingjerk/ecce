#ifndef GENERATORTABLES_HPP
#define GENERATORTABLES_HPP

#include "core.hpp"
#include "bitboards.hpp"

namespace Tables {
    void initTables();

    extern Bitboard::Type knightMasks[makeUNumspeed(1) << Coord::usedBits];
    extern Bitboard::Type kingMasks[makeUNumspeed(1) << Coord::usedBits];
    extern Bitboard::Type bishopMasks[makeUNumspeed(1) << Coord::usedBits];
    extern Bitboard::Type rookMasks[makeUNumspeed(1) << Coord::usedBits];
}

#endif /* GENERATORTABLES_HPP */
