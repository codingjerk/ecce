#ifndef SCORE_HPP
#define SCORE_HPP

#include <string>
#include <sstream>

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
	
	inline bool isPositiveMate(Type score) {
		return score >= (Infinity - Type(MAX_DEPTH));
	}
	
	inline bool isNegativeMate(Type score) {
		return score <= (-Infinity + Type(MAX_DEPTH));
	}

	inline bool isMate(Type score) {
		return isPositiveMate(score) || isNegativeMate(score);
	}

	inline std::string show(Type score) {
		std::stringstream ss;
		
		if (isPositiveMate(score)) {
			ss << "mate " << (score - Infinity + Type(MAX_DEPTH)) / 2;
		} else if (isNegativeMate(score)) {
			ss << "mate " << (score + Infinity - Type(MAX_DEPTH)) / 2;
		} else {
			ss << "cp " << score * 100 / Pawn;
		}

		return ss.str();
	}
}

#endif /* SCORE_HPP */
