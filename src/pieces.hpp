#ifndef PIECES_HPP
#define PIECES_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"

// Not in namespace to use White or Pawn without qualifitiers
using Color = UNumspeed;
const Color Black = makeUNumspeed(0);
const Color White = makeUNumspeed(8);

using Dignity = UNumspeed;
const Dignity rangedMask = 4;
const Dignity Pawn = 1; 
const Dignity Knight = 2; 
const Dignity King = 3; 
const Dignity Bishop = rangedMask /*4*/; 
const Dignity Rook = rangedMask + 1 /*5*/; 
const Dignity Queen = rangedMask + 2 /*6*/;

namespace Piece {
    using Type = UNumspeed;

    void initTables();

    Type create(const Color, const Dignity);
    Type fromChar(const char);
    Type fromString(const std::string);

    std::string show(const Type);

    const UNumspeed dignityLower   = makeNumspeed(1);
    const UNumspeed dignityHighter = makeNumspeed(6);

    // From 1 (pawn) to 6 (queen) - 3 bits total
    const UNumspeed usedBitsByDignity = makeUNumspeed(3);
    const UNumspeed dignityMask = (makeUNumspeed(1) << usedBitsByDignity) - makeUNumspeed(1);
    
    // 0 (black) or 8 (white) - 1 bit (at same position as 8)
    const UNumspeed usedBitsByColor = makeUNumspeed(1);
    const UNumspeed colorMask = makeUNumspeed(8);

    const UNumspeed usedBits = usedBitsByDignity + usedBitsByColor;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);
}

#endif /* PIECES_HPP */
