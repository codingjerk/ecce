//@TODO(GLOBAL): Store more info in move:
//coord from, to (12 bits)
//piece, that moves (4 bits)
//piece, that captured (4 bits)
//piece, that promoted (4 bits)
//total - 24 bits
//and we also can store movetype (8 bits - up to 256 types)
//types examples: simple, enpassant, 0-0-0, 0-0, capture, promotion, capture with promotion
//pawn doublemove, checkpossibles (with simple fuzzy calculations)

#include "moves.hpp"

#include "cstf.hpp"

using namespace Move;

Type Move::create(const Coord::Type from, const Coord::Type to, const Piece::Type captured) {
    ASSERT((from & Coord::typeMask) == from);
    ASSERT((to & Coord::typeMask) == to);

    return (captured << coordsBits) | (from << Coord::usedBits) | to;
}

Type Move::promotion(const Coord::Type from, const Coord::Type to, const Piece::Type promoted, const Piece::Type captured) {
    return create(from, to, captured) | (promoted << (coordsBits + captureBits));
}

Boolspeed Move::isPromotion(const Type move) {
    return move & promotionMask;
}

Boolspeed Move::isCapture(const Type move) {
    return move & captureMask;
}

Type Move::fromString(const std::string str) {
    ASSERT(str.size() == 4);

    return create(Coord::fromString(str.substr(0,2)), Coord::fromString(str.substr(2,2)));
}

std::string Move::show(const Type move) {
    return Coord::show((move >> Coord::usedBits) & Coord::typeMask) + Coord::show(move & Coord::typeMask);
}