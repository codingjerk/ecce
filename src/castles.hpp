#ifndef CASTLES_HPP
#define CASTLES_HPP

#include <string>

#include "core.hpp"
#include "colors.hpp"
#include "dignities.hpp"

namespace Castle {
    using Type = UNumspeed;

    Type fromString(const std::string);

    std::string show(const Type);
    
    const Type whiteKing  = makeUNumspeed(8);
    const Type whiteQueen = makeUNumspeed(4);
    const Type blackKing  = makeUNumspeed(2);
    const Type blackQueen = makeUNumspeed(1);

    const Type white = whiteKing | whiteQueen;
    const Type black = blackKing | blackQueen;

    const Type all = white | black;

    const Type null = 0;

    const UNumspeed usedBits = 4;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    inline Type create(const Boolspeed wk, const Boolspeed wq, const Boolspeed bk, const Boolspeed bq) {
        return (wk? whiteKing:  null)
             | (wq? whiteQueen: null)
             | (bk? blackKing:  null)
             | (bq? blackQueen: null);
    }

    template <Color::Type COLOR, Dignity::Type DIGNITY>
    inline Type is(Type);

    template <>
    inline Type is<White, King>(Type castle) {
        return castle & whiteKing;
    }

    template <>
    inline Type is<Black, King>(Type castle) {
        return castle & blackKing;
    }

    template <>
    inline Type is<White, Queen>(Type castle) {
        return castle & whiteQueen;
    }

    template <>
    inline Type is<Black, Queen>(Type castle) {
        return castle & blackQueen;
    }
}

#endif /* CASTLES_HPP */
