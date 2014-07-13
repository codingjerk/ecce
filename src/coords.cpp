#include "coords.hpp"
#include "cstf.hpp"

using namespace Coord;

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

    result[0] = (coord & componentMask) + 'a';
    result[1] = (coord >> usedBitsByComponent) + '1';

    return result; 
}
