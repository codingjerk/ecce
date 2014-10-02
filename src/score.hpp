#ifndef SCORE_HPP
#define SCORE_HPP

#include "core.hpp"

namespace Score {
    typedef Numspeed Type;

    const Type Null     = 0;
    const Type Draw     = 0;

    const Type Pawn     = 10000;
    const Type Bishop   = 30000;
    const Type Knight   = 30000;
    const Type Rook     = 50000;
    const Type Queen    = 90000;

    const Type Infinity = 100 * Queen;
}

#endif /* SCORE_HPP */
