#ifndef MOVES_HPP
#define MOVES_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"
#include "pieces.hpp"

namespace Move {
    using Type = UNumspeed;

    Type create(const Coord::Type, const Coord::Type, const UNumspeed flags = 0);
    Type promotion(const Coord::Type, const Coord::Type, const Piece::Type, const UNumspeed flags = 0);
    Type fromString(const std::string);

    bool isPromotion(const Type);
    bool isCapture(const Type);

    std::string show(const Type);

    const UNumspeed coordsBits = Coord::usedBits * makeUNumspeed(2);
    const UNumspeed coordsMask = (makeUNumspeed(1) << coordsBits) - makeUNumspeed(1);

    const UNumspeed captureBits = makeUNumspeed(1);
    const UNumspeed captureFlag = makeUNumspeed(1) << (Coord::usedBits * makeUNumspeed(2));
    const UNumspeed captureMask = captureFlag;

    const UNumspeed promotionBits = Piece::usedBits;
    const UNumspeed promotionMask = Piece::typeMask << (coordsBits + captureBits);

    const UNumspeed usedBits = coordsBits + captureBits + promotionBits;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);
}

#endif /* MOVES_HPP */
