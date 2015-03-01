#ifndef COORDS_HPP
#define COORDS_HPP

#include <string>

#include "core.hpp"

namespace Coord {
    using Type = UNumspeed; 

    const UNumspeed componentLower   = makeNumspeed(0);
    const UNumspeed componentHighter = makeNumspeed(7);

    const UNumspeed usedBitsByComponent = makeUNumspeed(3);
    const UNumspeed componentMask = (makeUNumspeed(1) << usedBitsByComponent) - makeUNumspeed(1);

    const UNumspeed usedBits = usedBitsByComponent * makeUNumspeed(2);
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    inline Type fromRaw(const UNumspeed rawX, const UNumspeed rawY) {
        return rawX | rawY;
    }

    Type fromChars(const char, const char);
    Type fromString(const std::string&);

    std::string show(const Type);

    inline Type create(const UNumspeed x, const UNumspeed y) {
        return fromRaw(x, y << usedBitsByComponent);
    }

    inline Type alpha(const Type coord) {
        return coord & componentMask;
    }

    inline Type digit(const Type coord) {
        return coord & (componentMask << usedBitsByComponent);
    }

    const Type A1 = 0;
    const Type B1 = 1;
    const Type C1 = 2;
    const Type D1 = 3;
    const Type E1 = 4;
    const Type F1 = 5;
    const Type G1 = 6;
    const Type H1 = 7;

    const Type A2 = 8 + 0;
    const Type B2 = 8 + 1;
    const Type C2 = 8 + 2;
    const Type D2 = 8 + 3;
    const Type E2 = 8 + 4;
    const Type F2 = 8 + 5;
    const Type G2 = 8 + 6;
    const Type H2 = 8 + 7;

    const Type A3 = 16 + 0;
    const Type B3 = 16 + 1;
    const Type C3 = 16 + 2;
    const Type D3 = 16 + 3;
    const Type E3 = 16 + 4;
    const Type F3 = 16 + 5;
    const Type G3 = 16 + 6;
    const Type H3 = 16 + 7;

    const Type A4 = 24 + 0;
    const Type B4 = 24 + 1;
    const Type C4 = 24 + 2;
    const Type D4 = 24 + 3;
    const Type E4 = 24 + 4;
    const Type F4 = 24 + 5;
    const Type G4 = 24 + 6;
    const Type H4 = 24 + 7;

    const Type A5 = 32 + 0;
    const Type B5 = 32 + 1;
    const Type C5 = 32 + 2;
    const Type D5 = 32 + 3;
    const Type E5 = 32 + 4;
    const Type F5 = 32 + 5;
    const Type G5 = 32 + 6;
    const Type H5 = 32 + 7;

    const Type A6 = 40 + 0;
    const Type B6 = 40 + 1;
    const Type C6 = 40 + 2;
    const Type D6 = 40 + 3;
    const Type E6 = 40 + 4;
    const Type F6 = 40 + 5;
    const Type G6 = 40 + 6;
    const Type H6 = 40 + 7;

    const Type A7 = 48 + 0;
    const Type B7 = 48 + 1;
    const Type C7 = 48 + 2;
    const Type D7 = 48 + 3;
    const Type E7 = 48 + 4;
    const Type F7 = 48 + 5;
    const Type G7 = 48 + 6;
    const Type H7 = 48 + 7;

    const Type A8 = 56 + 0;
    const Type B8 = 56 + 1;
    const Type C8 = 56 + 2;
    const Type D8 = 56 + 3;
    const Type E8 = 56 + 4;
    const Type F8 = 56 + 5;
    const Type G8 = 56 + 6;
    const Type H8 = 56 + 7;

    #define forRawXCoords(var) for (UNumspeed var = Coord::componentLower; var <= Coord::componentHighter; ++var)
    #define forRawYCoords(var) for (Numspeed var = makeNumspeed(56); var >= makeNumspeed(0); var -= 8)
    
    #define forCoord(var) for (UNumspeed var = Coord::componentLower; var <= Coord::componentHighter; ++var)
    #define forCoords(var) for (UNumspeed var = Coord::A1; var <= Coord::H8; ++var)
}

#endif /* COORDS_HPP */
