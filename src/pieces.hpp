#ifndef PIECES_HPP
#define PIECES_HPP

#include <string>

#include "core.hpp"

#include "colors.hpp"
#include "dignities.hpp"

namespace Piece {
    using Type = UNumspeed;

    void initTables();

    inline Type create(const Color::Type color, const Dignity::Type dignity) {
        return color | dignity;
    }

    Type fromChar(const char);
    Type fromString(const std::string&);

    const Type null = makeUNumspeed(0);

    std::string show(const Type);

    const UNumspeed usedBits = Dignity::usedBits + Color::usedBits;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);
}

#endif /* PIECES_HPP */
