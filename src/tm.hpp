#ifndef TM_HPP
#define TM_HPP

#include "core.hpp"

namespace TM {
    struct DepthLimit {
        Numspeed maxDepth = MAX_DEPTH;
    };

    struct TimeLimit {
        Numspeed maxTime;
    };

    inline DepthLimit depth(Numspeed aDepth) {
        DepthLimit result;

        result.maxDepth = aDepth;

        return result;
    }

    inline TimeLimit time(Numspeed aTime) {
        TimeLimit result;

        result.maxTime = aTime;

        return result;
    }

    inline TimeLimit time(Numspeed aTime, Numspeed inc, Numspeed moves) {
        if (moves != 0) {
            return time((aTime + inc * moves) / moves);
        } else {
            return time((aTime + inc * 40) / 40);
        }
    }
}

#endif /* TM_HPP */
