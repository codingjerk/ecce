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
         | (promoted << promotionOffset)
         | (promotionFlag << specialOffset);
}

template <Color::Type COLOR>
Type Move::enpassant(const Coord::Type from, const Coord::Type to) {
    return create(from, to, Piece::create(Color::inv(COLOR), Pawn)) 
         | (enpassantFlag << specialOffset);
}

Type Move::pawnDouble(const Coord::Type from, const Coord::Type to) {
    return create(from, to) 
         | (pawnDoubleFlag << specialOffset);
}

//@TODO: functions for special moves
template <> Type Move::castleLong<White>() {
    return create(Coord::fromString("e1"), Coord::fromString("c1")) 
         | (castleLongWhiteFlag << specialOffset);
}

template <> Type Move::castleLong<Black>() {
    return create(Coord::fromString("e8"), Coord::fromString("c8")) 
         | (castleLongBlackFlag << specialOffset);
}

template <> Type Move::castleShort<White>() {
    return create(Coord::fromString("e1"), Coord::fromString("g1")) 
         | (castleShortWhiteFlag << specialOffset);
}

template <> Type Move::castleShort<Black>() {
    return create(Coord::fromString("e8"), Coord::fromString("g8")) 
         | (castleShortBlackFlag << specialOffset);
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

Type Move::fromString(const std::string str) {
    ASSERT(str.size() == 4);

    return create(Coord::fromString(str.substr(0,2)), Coord::fromString(str.substr(2,2)));
}

std::string Move::show(const Type move) {
    return Coord::show((move >> Coord::usedBits) & Coord::typeMask) + Coord::show(move & Coord::typeMask);
}