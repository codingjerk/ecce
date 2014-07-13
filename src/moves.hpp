#ifndef MOVES_HPP
#define MOVES_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"
#include "pieces.hpp"

namespace Move {
    using Type = UNumspeed;

    Type fromString(const std::string);

    template <Color::Type COLOR> Type castleLong();
    template <Color::Type COLOR> Type castleShort();

    const UNumspeed enpassantFlag = makeNumspeed(1);
    const UNumspeed castleLongWhiteFlag = makeNumspeed(2);
    const UNumspeed castleShortWhiteFlag = makeNumspeed(3);
    const UNumspeed pawnDoubleFlag = makeNumspeed(4);
    const UNumspeed promotionFlag = makeNumspeed(5);
    const UNumspeed castleLongBlackFlag = makeNumspeed(6);
    const UNumspeed castleShortBlackFlag = makeNumspeed(7);

    std::string show(const Type);

    const UNumspeed coordsBits = Coord::usedBits * makeUNumspeed(2);
    const UNumspeed coordsOffset = makeUNumspeed(0);
    const UNumspeed coordsMask = (makeUNumspeed(1) << coordsBits) - makeUNumspeed(1);

    const UNumspeed specialBits = makeUNumspeed(4); // for 15 special moves
    const UNumspeed specialOffset = coordsOffset + coordsBits;
    const UNumspeed specialMask = ((makeUNumspeed(1) << specialBits) - makeUNumspeed(1)) << specialOffset;

    const UNumspeed captureBits = Piece::usedBits;
    const UNumspeed captureOffset = specialOffset + specialBits;
    const UNumspeed captureMask = Piece::typeMask << captureOffset;

    const UNumspeed promotionBits = Piece::usedBits;
    const UNumspeed promotionOffset = captureOffset + captureBits;
    const UNumspeed promotionMask = Piece::typeMask << promotionOffset;

    const UNumspeed usedBits = coordsBits + specialBits + captureBits + promotionBits;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    inline UNumspeed special(const Type move) {
        return (move & specialMask) >> specialOffset;
    }

    inline Boolspeed isCapture(const Type move) {
        return move & captureMask;
    }

    inline Type create(const Coord::Type from, const Coord::Type to, const Piece::Type captured = Piece::null) {
        return (captured << captureOffset) | (from << Coord::usedBits) | to;
    }

    inline Type promotion(const Coord::Type from, const Coord::Type to, const Piece::Type promoted, const Piece::Type captured = Piece::null) {
        return create(from, to, captured) 
             | (promoted << promotionOffset)
             | (promotionFlag << specialOffset);
    }

    inline Type pawnDouble(const Coord::Type from, const Coord::Type to) {
        return create(from, to) 
             | (pawnDoubleFlag << specialOffset);
    }

    template <Color::Type COLOR>
    Type enpassant(const Coord::Type from, const Coord::Type to) {
        return create(from, to, Piece::create(Color::inv(COLOR), Pawn)) 
             | (enpassantFlag << specialOffset);
    }
}

#endif /* MOVES_HPP */
