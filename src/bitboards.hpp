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
        #ifdef OSLINUX
            asm("bsfq %0, %0": "=r" (bitboard): "0" (bitboard));
            return bitboard;
        #else
            unsigned long result;
            _BitScanForward64(&result, bitboard);
            return result;
        #endif
    }

    inline UNumspeed enabledCount(Type bitboard) {
        #ifdef OSLINUX
            return (bitboard * 0x0101010101010101ull) >> 56ull;
        #else
            return __popcnt64(bitboard);
        #endif
    }

    extern Type upLine;
    extern Type downLine;
    extern Type leftLine;
    extern Type rightLine;

    std::string show(const Type);
}

#endif /* BITBOARDS_HPP */
