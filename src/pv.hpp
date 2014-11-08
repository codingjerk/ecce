#ifndef PV_HPP
#define PV_HPP

#include "core.hpp"
#include "moves.hpp"

namespace PV {
    const UNumspeed PV_SIZE = (MAX_DEPTH * MAX_DEPTH + MAX_DEPTH) / 2;
    static Move::Type master[PV_SIZE];

    inline void copy(Move::Type *dst, const Move::Type *src, UNumspeed n) {
        while (n-- && (*dst++ = *src++));
    }

    inline std::string show() {
        std::string result;

        Move::Type move;
        for (UNumspeed i = 0; (move = master[i]); ++i) {
            result += Move::show(move) + " ";
        }

        return result;
    }
}

#endif /* PV_HPP */
