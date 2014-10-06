#ifndef SCORE_HPP
#define SCORE_HPP

#include <string>
#include <sstream>

#include "core.hpp"

namespace Score {
    typedef Numspeed Type;

    const Type Null     = 0;
    const Type Draw     = 0;

    const Type Pawn     = 100;
    const Type Bishop   = 300;
    const Type Knight   = 300;
    const Type Rook     = 500;
    const Type Queen    = 900;

    const Type Infinity = 20 * Queen;

	inline Type makeMate(UNumspeed ply) {
		return Infinity - Type(MAX_DEPTH) + Type(ply);
	}
	
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
			ss << "mate " << (score - Infinity + Type(MAX_DEPTH) + 1) / 2;
		} else if (isNegativeMate(score)) {
			ss << "mate " << (score + Infinity - Type(MAX_DEPTH) - 1) / 2;
		} else {
			ss << "cp " << score;
		}

		return ss.str();
	}
}

#endif /* SCORE_HPP */
