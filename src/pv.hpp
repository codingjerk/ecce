#ifndef PV_HPP
#define PV_HPP

#include "core.hpp"
#include "moves.hpp"

namespace PV {
    const UNumspeed PV_SIZE = (MAX_DEPTH * MAX_DEPTH + MAX_DEPTH) / 2;
    Move::Type master[PV_SIZE];

    inline void copy(Move::Type *pTarget, const Move::Type *pSource, UNumspeed n) {
        while (n-- && (*pTarget++ = *pSource++));
    }

    inline std::string show() {
        std::string result;

        Move::Type move;
        for (int i = 0; move = master[i]; ++i) {
            result += Move::show(move) + " ";
        }

        return result;
    }

    inline void clear() {
        for (int i = 0; i < PV_SIZE; ++i) {
            master[i] = 0;
        }
    }
}

#endif /* PV_HPP */
