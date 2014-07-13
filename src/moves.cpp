#include "moves.hpp"

#include "cstf.hpp"

using namespace Move;

template <> Type Move::castleLong<White>() {
    return create(Coord::E1, Coord::C1) 
         | (castleLongFlag << specialOffset);
}

template <> Type Move::castleLong<Black>() {
    return create(Coord::E8, Coord::C8) 
         | (castleLongFlag << specialOffset);
}

template <> Type Move::castleShort<White>() {
    return create(Coord::E1, Coord::G1) 
         | (castleShortFlag << specialOffset);
}

template <> Type Move::castleShort<Black>() {
    return create(Coord::E8, Coord::G8) 
         | (castleShortFlag << specialOffset);
}

Type Move::fromString(const std::string str) {
    ASSERT(str.size() == 4);

    return create(Coord::fromString(str.substr(0,2)), Coord::fromString(str.substr(2,2)));
}

std::string Move::show(const Type move) {
    return Coord::show((move >> Coord::usedBits) & Coord::typeMask) + Coord::show(move & Coord::typeMask);
}
