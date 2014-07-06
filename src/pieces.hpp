#ifndef PIECES_HPP
#define PIECES_HPP

#include <string>

#include "core.hpp"

#include "colors.hpp"
#include "dignities.hpp"

namespace Piece {
    using Type = UNumspeed;

    void initTables();

    Type create(const Color::Type, const Dignity::Type);
    Type fromChar(const char);
    Type fromString(const std::string);

    std::string show(const Type);

    const UNumspeed usedBits = Dignity::usedBits + Color::usedBits;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);
}

#endif /* PIECES_HPP */
