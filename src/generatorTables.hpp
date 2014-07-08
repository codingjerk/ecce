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

    extern Bitboard::Type whiteKingCastleNeeded;
    extern Coord::Type whiteKingCastleTarget;
    
    extern Bitboard::Type whiteQueenCastleNeeded;
    extern Coord::Type whiteQueenCastleTarget;
    
    extern Bitboard::Type blackKingCastleNeeded;
    extern Coord::Type blackKingCastleTarget;
    
    extern Bitboard::Type blackQueenCastleNeeded;
    extern Coord::Type blackQueenCastleTarget;
}

#endif /* GENERATORTABLES_HPP */
