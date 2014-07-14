#include "castles.hpp"

#include "cstf.hpp"

using namespace Castle;

Type Castle::fromString(const std::string str) {
    if (str[0] == '-') {
        ASSERT(str == "-");

        return null;
    } 

    Type result = null;
    UNumspeed cursor = 0;

    if (str[cursor] == 'K') { result |= whiteKing;  ++cursor; }
    if (str[cursor] == 'Q') { result |= whiteQueen; ++cursor; }
    if (str[cursor] == 'k') { result |= blackKing;  ++cursor; }
    if (str[cursor] == 'q') { result |= blackQueen; ++cursor; }

    ASSERT(cursor == str.size());

    return result;
}

std::string Castle::show(const Type castle) {
    if (castle == null) return "-";

    std::string result;
    result.reserve(4);

    if (castle & whiteKing)  result += 'K'; 
    if (castle & whiteQueen) result += 'Q';
    if (castle & blackKing)  result += 'k'; 
    if (castle & blackQueen) result += 'q';

    return result;
}
