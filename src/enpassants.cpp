#include "enpassants.hpp"

#include <limits.h>

#include "cstf.hpp"

using namespace Enpassant;

Type Enpassant::create(const Coord::Type coord) {
    return coord;
}

Type Enpassant::fromString(const std::string str) {
    if (str.size() == 1) {
        ASSERT(str == "-");
        return null;
    } else {
        return Coord::fromString(str);
    }
}

std::string Enpassant::show(const Type enpassant) {
    if (enpassant & null) {
        return "-";
    } else {
        return Coord::show(enpassant);
    }
}
