#ifndef TM_HPP
#define TM_HPP

#include "core.hpp"

namespace TM {
	struct DepthLimit {
		Numspeed maxDepth = MAX_DEPTH;
	};

	DepthLimit depth(Numspeed aDepth) {
        DepthLimit result;

		result.maxDepth = aDepth;

		return result;
    }
}

#endif /* TM_HPP */
