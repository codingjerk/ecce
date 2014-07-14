#ifndef SCORES_HPP
#define SCORES_HPP

#include "core.hpp"

namespace Score {
    using Type = UNumspeed;

    const Type Pawn   = 10000;
    const Type Knight = 30000;
    const Type Bishop = 30000;
    const Type Rook   = 50000;
    const Type Queen  = 90000;
    const Type King   = 0;

    const Type Infinity = 64 * Queen; // Realy big number
}

#endif /* SCORES_HPP */
