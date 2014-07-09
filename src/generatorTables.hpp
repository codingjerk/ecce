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

    extern Bitboard::Type pawnStartLine[makeUNumspeed(1) << Color::usedBitsReal];

    extern Bitboard::Type castleNeeded[makeUNumspeed(1) << Color::usedBitsReal][makeUNumspeed(1) << Dignity::usedBits];
    extern Coord::Type castleTarget[makeUNumspeed(1) << Color::usedBitsReal][makeUNumspeed(1) << Dignity::usedBits];
}

#endif /* GENERATORTABLES_HPP */
