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

    // One number from 0 to 7 eats 3 bits
    const UNumspeed usedBitsByComponent = makeUNumspeed(3);
    const UNumspeed componentMask = (makeUNumspeed(1) << usedBitsByComponent) - makeUNumspeed(1);

    const UNumspeed usedBits = usedBitsByComponent * 2;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1); 
}

#endif /* COORDS_HPP */
