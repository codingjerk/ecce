#ifndef MOVES_HPP
#define MOVES_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"
#include "pieces.hpp"

namespace Move {
    using Type = UNumspeed;

    Type create(const Coord::Type, const Coord::Type, const Piece::Type captured = Piece::null);
    Type fromString(const std::string);

    Type promotion(const Coord::Type, const Coord::Type, const Piece::Type, const Piece::Type = Piece::null);
    
    template <Color::Type COLOR> Type castleLong();
    template <Color::Type COLOR> Type castleShort();
    template <Color::Type COLOR> Type enpassant(const Coord::Type, const Coord::Type);

    Boolspeed isPromotion(const Type);
    Boolspeed isCapture(const Type);
    Boolspeed isEnpassant(const Type);
    Boolspeed isCastleLong(const Type);
    Boolspeed isCastleShort(const Type);

    const UNumspeed enpassantFlag = makeNumspeed(1);
    const UNumspeed castleLongFlag = makeNumspeed(2);
    const UNumspeed castleShortFlag = makeNumspeed(3);

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
}

#endif /* MOVES_HPP */
