#include "castles.hpp"

#include <limits.h>

#include "cstf.hpp"

using namespace Castle;

Type Castle::create(const Boolspeed wk, const Boolspeed wq, const Boolspeed bk, const Boolspeed bq) {
    return (wk? whiteKing:  null)
         | (wq? whiteQueen: null)
         | (bk? blackKing:  null)
         | (bq? blackQueen: null);
}

Type Castle::fromString(const std::string str) {
    if (str.size() == 1) {
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

// Temporary solution
namespace Castle {
template <>
Type is<White, King>(Type castle) {
    return castle & whiteKing;
}

template <>
Type is<Black, King>(Type castle) {
    return castle & blackKing;
}

template <>
Type is<White, Queen>(Type castle) {
    return castle & whiteQueen;
}

template <>
Type is<Black, Queen>(Type castle) {
    return castle & blackQueen;
}
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
