#ifndef COLORS_HPP
#define COLORS_HPP

#include <string>

#include "core.hpp"

namespace Color {
    using Type = UNumspeed;
    const Type Black = makeUNumspeed(0);
    const Type White = makeUNumspeed(8);

    void initTables();

    Type fromChar(const char);
    Type fromString(const std::string);

    std::string show(const Type);

    const UNumspeed usedBits = makeUNumspeed(1);
    const UNumspeed usedBitsReal = makeUNumspeed(4);
    const UNumspeed typeMask = makeUNumspeed(8);
}

using Color::Black;
using Color::White;

#endif /* COLORS_HPP */
