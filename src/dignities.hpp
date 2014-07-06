#ifndef DIGNITIES_HPP
#define DIGNITIES_HPP

#include <string>

#include "core.hpp"

namespace Dignity {
    using Type = UNumspeed;
    const Type rangedMask = makeUNumspeed(4);
    const Type Pawn   = makeUNumspeed(1); 
    const Type Knight = makeUNumspeed(2); 
    const Type King   = makeUNumspeed(3); 
    const Type Bishop = rangedMask /*4*/; 
    const Type Rook   = rangedMask + makeUNumspeed(1) /*5*/; 
    const Type Queen  = rangedMask + makeUNumspeed(2) /*6*/;

    const UNumspeed usedBits = makeUNumspeed(3);
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    const UNumspeed lower   = makeNumspeed(1);
    const UNumspeed highter = makeNumspeed(6);

    #define forDignities(var) for (Dignity::Type dignity = Dignity::lower; dignity <= Dignity::highter; ++dignity)
}

using Dignity::Pawn;
using Dignity::Knight;
using Dignity::King;
using Dignity::Bishop;
using Dignity::Rook;
using Dignity::Queen;

#endif /* DIGNITIES_HPP */
