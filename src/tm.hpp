#ifndef TM_HPP
#define TM_HPP

#include "core.hpp"

#include <algorithm>

namespace TM {
    struct DepthLimit {
        Numspeed maxDepth = MAX_DEPTH;
    };

    struct TimeLimit {
        Numspeed minTime;
        Numspeed real;
        Numspeed maxTime;
    };

    inline DepthLimit depth(Numspeed aDepth) {
        DepthLimit result;

        result.maxDepth = aDepth;

        return result;
    }

    inline TimeLimit time(Numspeed aTime) {
        TimeLimit result;

        result.minTime = aTime;
        result.real    = aTime;
        result.maxTime = aTime;

        return result;
    }

    inline TimeLimit time(Numspeed aTime, Numspeed inc, Numspeed moves) {
        TimeLimit result;

        if (moves != 0) {
            result.real = (aTime + inc * moves) / moves;
        } else {
            result.real = (aTime + inc * 40) / 40;
        }

        result.minTime = result.real / 3;
        result.maxTime = std::min(result.real * 3, aTime - 1);

        return result;
    }

    inline Numspeed timeUp(TimeLimit& tl) {
        auto told = tl.real;
        tl.real = (tl.real + tl.maxTime) / 2;

        return tl.real - told;
    }

    inline Numspeed timeDown(TimeLimit& tl) {
        auto told = tl.real;
        tl.real = (tl.real + tl.minTime) / 2;

        return told - tl.real;
    }
}

#endif /* TM_HPP */
