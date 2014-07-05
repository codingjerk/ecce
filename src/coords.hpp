#ifndef COORDS_HPP
#define COORDS_HPP

#include "core.hpp"

namespace Coord {
    using Type = UNumspeed;

    const UNumspeed usedBits = makeUNumspeed(8);

    Type fromChars(char x, char y);
    // std::string show();
}

#endif /* COORDS_HPP */
