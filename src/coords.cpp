#include "coords.hpp"
#include "cstf.hpp"

using namespace Coord;

Type Coord::create(const UNumspeed x, const UNumspeed y) {
    ASSERT(0 <= x && x <= 7);
    ASSERT(0 <= y && y <= 7);

    return (x << usedBitsByComponent) | y;
}

Type Coord::fromChars(const char x, const char y) {
    ASSERT('a' <= x && x <= 'h');
    ASSERT('1' <= y && y <= '8');

    return create(x - 'a', y - '1');
}

Type Coord::fromString(const std::string str) {
    ASSERT(str.size() == 2);

    return fromChars(str[0], str[1]);
}

std::string Coord::show(const Type coord) {
    std::string result("  ");

    result[0] = (coord >> usedBitsByComponent) + 'a';
    result[1] = (coord & componentMask) + '1';

    return result; 
}
