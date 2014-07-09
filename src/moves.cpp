#include "moves.hpp"

#include "cstf.hpp"

using namespace Move;

Type Move::create(const Coord::Type from, const Coord::Type to, UNumspeed flags) {
    ASSERT((from & Coord::typeMask) == from);
    ASSERT((to & Coord::typeMask) == to);

    return (from << Coord::usedBits) | to | flags;
}

Type Move::promotion(const Coord::Type from, const Coord::Type to, const Piece::Type piece, const UNumspeed flags) {
    return create(from, to, flags | (piece << (coordsBits + captureBits)));
}

bool Move::isPromotion(const Type move) {
    return move & promotionMask;
}

bool Move::isCapture(const Type move) {
    return move & captureFlag;
}

Type Move::fromString(const std::string str) {
    ASSERT(str.size() == 4);

    return create(Coord::fromString(str.substr(0,2)), Coord::fromString(str.substr(2,2)));
}

std::string Move::show(const Type move) {
    return Coord::show(move >> Coord::usedBits) + Coord::show(move & Coord::typeMask);
}
