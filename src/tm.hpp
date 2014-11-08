#ifndef TM_HPP
#define TM_HPP

#include "core.hpp"

#include <algorithm>

namespace TM {
    struct DepthLimit {
        Numspeed maxDepth = MAX_DEPTH;
    };

    struct TimeLimit {
        Numspeed real;
    };

    inline DepthLimit depth(Numspeed aDepth) {
        DepthLimit result;

        result.maxDepth = aDepth;

        return result;
    }

    inline TimeLimit time(Numspeed aTime) {
        TimeLimit result;

        result.real = aTime;

        return result;
    }

    inline TimeLimit time(Numspeed aTime, Numspeed inc, Numspeed moves) {
        TimeLimit result;

        if (moves != 0) {
            result.real = (aTime + inc * moves) / moves;
        } else {
            result.real = (aTime + inc * 40) / 40;
        }

        return result;
    }
}

#endif /* TM_HPP */
