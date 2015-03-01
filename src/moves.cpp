#include "moves.hpp"

#include "cstf.hpp"

using namespace Move;

Type Move::fromString(const std::string &str) {
    ASSERT(str.size() == 4);

    return create(Coord::fromString(str.substr(0,2)), Coord::fromString(str.substr(2,2)));
}

std::string Move::show(const Type move) {
    return Coord::show((move >> Coord::usedBits) & Coord::typeMask) + Coord::show(move & Coord::typeMask);
}
