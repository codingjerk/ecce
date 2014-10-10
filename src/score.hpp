#ifndef SCORE_HPP
#define SCORE_HPP

#include <string>
#include <sstream>

#include "core.hpp"
#include "pieces.hpp"

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

    extern Type pieceToScoreTable[makeUNumspeed(1) << Piece::usedBits];

    void initTables();

    inline Type makeMate(UNumspeed ply) {
        return Infinity - Type(ply);
    }
    
    inline bool isPositiveMate(Type score) {
        return score >= (Infinity - Type(MAX_DEPTH));
    }
    
    inline bool isNegativeMate(Type score) {
        return score <= (Type(MAX_DEPTH) - Infinity);
    }

    inline bool isMate(Type score) {
        return isPositiveMate(score) || isNegativeMate(score);
    }

    inline Numspeed getMateScore(Type score) {
        if (isPositiveMate(score)) {
            return (Infinity - score) / 2;
        } else {
            return (-score - Infinity) / 2;
        }
    }

    inline std::string show(Type score) {
        std::stringstream ss;
        
        if (isMate(score)) {
            ss << "mate " << getMateScore(score);
        } else {
            ss << "cp " << score;
        }

        return ss.str();
    }
}

#endif /* SCORE_HPP */
