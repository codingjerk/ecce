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

    DepthLimit depth(Numspeed aDepth) {
        DepthLimit result;

        result.maxDepth = aDepth;

        return result;
    }

    TimeLimit time(Numspeed aTime) {
        TimeLimit result;

        result.maxTime = aTime;

        return result;
    }

	TimeLimit time(Numspeed time, Numspeed inc, Numspeed moves) {
        TimeLimit result;

		if (moves != 0)
			result.maxTime = (time + inc * moves) / moves;
		else {
			result.maxTime = (time + inc * 40) / 40;
		}

        return result;
    }
}

#endif /* TM_HPP */
