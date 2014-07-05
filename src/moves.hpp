#ifndef MOVES_HPP
#define MOVES_HPP

#include <string>

#include "core.hpp"
#include "coords.hpp"

namespace Move {
    using Type = UNumspeed;

    Type create(const Coord::Type, const Coord::Type);
    Type fromString(const std::string);

    std::string show(const Type);

    const UNumspeed usedBits = Coord::usedBits * 2;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1); 
}

#endif /* MOVES_HPP */
