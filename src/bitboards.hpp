#ifndef BITBOARDS_HPP
#define BITBOARDS_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"
#include "moves.hpp"

namespace Bitboard {
    using Type = UNum64;

    void initTables();

    inline Type fromCoord(const Coord::Type coord) {
        return makeUNum64(1) << coord;
    }

    inline Type fromIndex(const UNumspeed index) {
        return makeUNum64(1) << index;
    }

    UNumspeed bitScan(Type);

    UNumspeed enabledCount(Type);

    const Type null = makeUNum64(0);
    const Type full = ~makeUNum64(0);

    extern Type upLine;
    extern Type downLine;
    extern Type leftLine;
    extern Type rightLine;

    std::string show(const Type);
}

#endif /* BITBOARDS_HPP */
