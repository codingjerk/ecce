#ifndef COORDS_HPP
#define COORDS_HPP

#include <string>

#include "core.hpp"

namespace Coord {
    using Type = UNumspeed; 

    inline Type fromRaw(const UNumspeed rawX, const UNumspeed rawY) {
        return rawX | rawY;
    }

    Type fromChars(const char, const char);
    Type fromString(const std::string);

    std::string show(const Type);

    const UNumspeed componentLower   = makeNumspeed(0);
    const UNumspeed componentHighter = makeNumspeed(7);

    // One number from 0 to 7 eats 3 bits
    const UNumspeed usedBitsByComponent = makeUNumspeed(3);
    const UNumspeed componentMask = (makeUNumspeed(1) << usedBitsByComponent) - makeUNumspeed(1);

    const UNumspeed usedBits = usedBitsByComponent * makeUNumspeed(2);
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    inline Type create(const UNumspeed x, const UNumspeed y) {
        return fromRaw(x, y << usedBitsByComponent);
    }

    #define forRawXCoords(var) for (UNumspeed var = Coord::componentLower; var <= Coord::componentHighter; ++var)
    #define forRawYCoords(var) for (Numspeed var = makeNumspeed(56); var >= makeNumspeed(0); var -= 8)
    
    #define forCoord(var) for (UNumspeed var = Coord::componentLower; var <= Coord::componentHighter; ++var)
}

#endif /* COORDS_HPP */
