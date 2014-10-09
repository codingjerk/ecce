#ifndef PST_HPP
#define PST_HPP

#include "pieces.hpp"
#include "score.hpp"

namespace PST {
    void initTables();

    extern Score::Type *tables[makeUNumspeed(1) << Piece::usedBits];
}

#endif /* PST_HPP */
