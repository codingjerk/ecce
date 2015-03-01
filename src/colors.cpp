#include "colors.hpp"

#include <limits.h>

#include "cstf.hpp"

using namespace Color;

Type charToColorTable[makeUNumspeed(1) << (sizeof(char) * CHAR_BIT)];
char colorToCharTable[makeUNumspeed(1) << Color::usedBitsReal];

Type Color::fromChar(const char c) {
    ASSERT(c == 'w' || c == 'b');

    return charToColorTable[(unsigned char)(c)];
}

Type Color::fromString(const std::string &str) {
    ASSERT(str.size() == 1);

    return fromChar(str[0]);
}

std::string Color::show(const Type color) {
    std::string result;
    result.reserve(1);

    result += colorToCharTable[color];

    return result;
}

#define UC(c) (unsigned char)(c)

void Color::initTables() {
    colorToCharTable[Black] = 'b';
    colorToCharTable[White] = 'w';

    charToColorTable[UC('b')] = Black;
    charToColorTable[UC('w')] = White;
}
