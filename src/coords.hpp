#ifndef COORDS_HPP
#define COORDS_HPP

#include <string>

#include "core.hpp"

namespace Coord {
    using Type = UNumspeed;

    const UNumspeed usedBits = makeUNumspeed(8);
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1); 

    const UNumspeed usedBitsByComponent = makeUNumspeed(4);
    const UNumspeed componentMask = (makeUNumspeed(1) << usedBitsByComponent) - makeUNumspeed(1); 

    Type create(UNumspeed x, UNumspeed y);

    Type fromChars(char x, char y);
    Type fromString(std::string str);

    std::string show(Type);
}

#endif /* COORDS_HPP */
