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

    void invert(Type&);
    
    std::string show(const Type);

    const UNumspeed usedBits = makeUNumspeed(1);
    const UNumspeed usedBitsReal = makeUNumspeed(4);
    const UNumspeed typeMask = makeUNumspeed(8);

    #define forColors(var) for (Color::Type color = Black; color <= White; color += White)
}

using Color::Black;
using Color::White;

#endif /* COLORS_HPP */