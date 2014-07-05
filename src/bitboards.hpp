#ifndef BITBOARDS_HPP
#define BITBOARDS_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"
#include "moves.hpp"

namespace Bitboard {
    using Type = UNum64;
    using LineType = UNumspeed;
    using BitType = UNumspeed;

    Type fromCoord(const Coord::Type);
    Type fromMove(const Move::Type);

    bool getBit(const Type, const UNumspeed, const UNumspeed);

    void initTables();

    std::string show(const Type);
}

#endif /* BITBOARDS_HPP */
