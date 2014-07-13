#ifndef CASTLES_HPP
#define CASTLES_HPP

#include <string>

#include "core.hpp"
#include "colors.hpp"
#include "dignities.hpp"

namespace Castle {
    using Type = UNumspeed;

    Type create(const Boolspeed, const Boolspeed, const Boolspeed, const Boolspeed);
    Type fromString(const std::string);

    std::string show(const Type);
    
    const Type whiteKing  = makeUNumspeed(8);
    const Type whiteQueen = makeUNumspeed(4);
    const Type blackKing  = makeUNumspeed(2);
    const Type blackQueen = makeUNumspeed(1);

    const Type white = whiteKing | whiteQueen;
    const Type black = blackKing | blackQueen;

    const UNumspeed usedBits = 4;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    template <Color::Type COLOR, Dignity::Type DIGNITY>
    Type is(Type);

    const Type null = 0;
}

#endif /* CASTLES_HPP */
