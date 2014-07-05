#ifndef COORDS_HPP
#define COORDS_HPP

#include <string>

#include "core.hpp"

namespace Coord {
    using Type = UNumspeed; 

    Type create(const UNumspeed, const UNumspeed);
    Type fromChars(const char, const char);
    Type fromString(const std::string);

    std::string show(const Type);

    const UNumspeed usedBits = makeUNumspeed(8);
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1); 

    const UNumspeed usedBitsByComponent = makeUNumspeed(4);
    const UNumspeed componentMask = (makeUNumspeed(1) << usedBitsByComponent) - makeUNumspeed(1);
}

#endif /* COORDS_HPP */
