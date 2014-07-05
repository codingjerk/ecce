#include "moves.hpp"

#include "cstf.hpp"

using namespace Move;

Type Move::create(const Coord::Type from, const Coord::Type to) {
    ASSERT((from & Coord::typeMask) == from);
    ASSERT((to & Coord::typeMask) == to);

    return (from << Coord::usedBits) | to;
}

Type Move::fromString(const std::string str) {
    ASSERT(str.size() == 4);

    return create(Coord::fromString(str.substr(0,2)), Coord::fromString(str.substr(2,2)));
}

std::string Move::show(const Type move) {
    return Coord::show(move >> Coord::usedBits) + Coord::show(move & Coord::typeMask);
}
