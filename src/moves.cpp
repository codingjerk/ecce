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

    return (captured << captureOffset) | (from << Coord::usedBits) | to;
}

Type Move::promotion(const Coord::Type from, const Coord::Type to, const Piece::Type promoted, const Piece::Type captured) {
    return create(from, to, captured) 
         | (promoted << promotionOffset);
}

template <Color::Type COLOR>
Type Move::enpassant(const Coord::Type from, const Coord::Type to) {
    return create(from, to, Piece::create(Color::inv(COLOR), Pawn)) 
         | (enpassantFlag << specialOffset);
}

//@TODO: functions for special moves
template <> Type Move::castleLong<White>() {
    return create(Coord::fromString("e1"), Coord::fromString("c1")) 
         | (castleLongFlag << specialOffset);
}

template <> Type Move::castleLong<Black>() {
    return create(Coord::fromString("e8"), Coord::fromString("c8")) 
         | (castleLongFlag << specialOffset);
}

template <> Type Move::castleShort<White>() {
    return create(Coord::fromString("e1"), Coord::fromString("g1")) 
         | (castleShortFlag << specialOffset);
}

template <> Type Move::castleShort<Black>() {
    return create(Coord::fromString("e8"), Coord::fromString("g8")) 
         | (castleShortFlag << specialOffset);
}

UNumspeed Move::special(const Type move) {
    return (move & specialMask) >> specialOffset;
}

template Type Move::enpassant<White>(const Coord::Type from, const Coord::Type to);
template Type Move::enpassant<Black>(const Coord::Type from, const Coord::Type to);

Boolspeed Move::isPromotion(const Type move) {
    return move & promotionMask;
}

Boolspeed Move::isCapture(const Type move) {
    return move & captureMask;
}

//@TODO: Flag specialFlag(Move)
Boolspeed Move::isEnpassant(const Type move) {
    return ((move & specialMask) >> specialOffset) == enpassantFlag;
}

Boolspeed Move::isCastleLong(const Type move) {
    return ((move & specialMask) >> specialOffset) == castleLongFlag;
}

Boolspeed Move::isCastleShort(const Type move) {
    return ((move & specialMask) >> specialOffset) == castleShortFlag;
}

Type Move::fromString(const std::string str) {
    ASSERT(str.size() == 4);

    return create(Coord::fromString(str.substr(0,2)), Coord::fromString(str.substr(2,2)));
}

std::string Move::show(const Type move) {
    return Coord::show((move >> Coord::usedBits) & Coord::typeMask) + Coord::show(move & Coord::typeMask);
}