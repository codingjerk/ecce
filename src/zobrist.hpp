#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP

#include "pieces.hpp"
#include "coords.hpp"

namespace Zobrist {
    using Type = UNum64; 

    void initTables();

    extern Type table[makeUNumspeed(1) << Piece::usedBits][makeUNumspeed(1) << Coord::usedBits];
    extern Type turnKey;
}

#endif /* ZOBRIST_HPP */
