#ifndef BITBOARDS_HPP
#define BITBOARDS_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"

namespace Bitboard {
    using Type = UNum64;

    void initTables();

    const Type null = makeUNum64(0);
    const Type full = ~makeUNum64(0);

    inline Type fromCoord(const Coord::Type coord) {
        return makeUNum64(1) << coord;
    }

    inline Type fromIndex(const UNumspeed index) {
        return makeUNum64(1) << index;
    }

    inline UNumspeed bitScan(Type bitboard) {
        asm("bsfq %0, %0": "=r" (bitboard): "0" (bitboard));
        return bitboard;
    }

    inline UNumspeed enabledCount(Type bitboard) {
        UNumspeed result = 0;

        while (bitboard != Bitboard::null) {
            auto const bit = bitScan(bitboard);
            bitboard ^= fromIndex(bit);

            ++result;
        }

        return result;
    }

    extern Type upLine;
    extern Type downLine;
    extern Type leftLine;
    extern Type rightLine;

    std::string show(const Type);
}

#endif /* BITBOARDS_HPP */
