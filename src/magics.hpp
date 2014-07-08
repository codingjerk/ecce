#ifndef MAGICS_HPP
#define MAGICS_HPP

#include "core.hpp"
#include "bitboards.hpp"

namespace Magic {
    void initTables();

    extern Bitboard::Type *bishopData;
    extern UNumspeed bishopOffsets[64];
    extern const UNum64 bishopMagics[makeUNumspeed(1) << Coord::usedBits];
    extern const UNumspeed bishopMaskBits[64];
    extern const UNumspeed bishopMaskShifts[64];
    
    extern Bitboard::Type *rookData;
    extern UNumspeed rookOffsets[makeUNumspeed(1) << Coord::usedBits];
    extern const UNum64 rookMagics[makeUNumspeed(1) << Coord::usedBits];
    extern const UNumspeed rookMaskBits[makeUNumspeed(1) << Coord::usedBits];
    extern const UNumspeed rookMaskShifts[makeUNumspeed(1) << Coord::usedBits];
}

#endif /* MAGICS_HPP */
