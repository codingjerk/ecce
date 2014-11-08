#ifndef DIGNITIES_HPP
#define DIGNITIES_HPP

#include "core.hpp"

namespace Dignity {
    using Type = UNumspeed;
    const Type Pawn   = makeUNumspeed(1); 
    const Type Knight = makeUNumspeed(2); 
    const Type Bishop = makeUNumspeed(3);
    const Type Rook   = makeUNumspeed(4);
    const Type Queen  = makeUNumspeed(5);
    const Type King   = makeUNumspeed(6); 

    const UNumspeed usedBits = makeUNumspeed(3);
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    const UNumspeed lower   = makeNumspeed(1);
    const UNumspeed highter = makeNumspeed(6);

    #define forDignities(var) for (Dignity::Type var = Dignity::lower; var <= Dignity::highter; ++var)
}

using Dignity::Pawn;
using Dignity::Knight;
using Dignity::King;
using Dignity::Bishop;
using Dignity::Rook;
using Dignity::Queen;

#endif /* DIGNITIES_HPP */
