#ifndef BITBOARDS_HPP
#define BITBOARDS_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"
#include "moves.hpp"

namespace Bitboard {
    using Type = UNum64;

    void initTables();

    Type fromCoord(const Coord::Type);
    Type fromMove(const Move::Type);

    std::string show(const Type);
}

#endif /* BITBOARDS_HPP */
