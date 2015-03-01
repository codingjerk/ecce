#ifndef ENPASSANTS_HPP
#define ENPASSANTS_HPP

#include <string>

#include "core.hpp"
#include "coords.hpp"

namespace Enpassant {
    using Type = Coord::Type;

    inline Type create(const Coord::Type coord) {
        return coord;
    }

    Type fromString(const std::string&);

    std::string show(const Type);

    const UNumspeed usedBits = Coord::usedBits + 1;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    const Type null = makeUNumspeed(1) << usedBits;
}

#endif /* ENPASSANTS_HPP */
