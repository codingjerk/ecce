#include "bitboards.hpp"
#include "cstf.hpp"

using namespace Bitboard;

Type Bitboard::upLine;
Type Bitboard::downLine;
Type Bitboard::leftLine;
Type Bitboard::rightLine;

Type coordToBitboardTable[makeUNumspeed(1) << Coord::usedBits];
Type moveToBitboardTable[makeUNumspeed(1) << Move::usedBits];

Type Bitboard::fromMove(const Move::Type move) {
    ASSERT((move & Move::typeMask) == move);

    return moveToBitboardTable[move];
}

std::string Bitboard::show(const Type bitboard) {
    std::string result;
    result.reserve(64+8);

    forRawYCoords(y) {
        forRawXCoords(x) {
            result += '0' + bool(bitboard & fromCoord(Coord::fromRaw(x, y)));
        }

        result += '\n';
    }

    return result;
}

UNumspeed Bitboard::bitScan(Type bitboard) {
    ASSERT(bitboard != Bitboard::null);

    asm("bsfq %0, %0": "=r" (bitboard): "0" (bitboard));
    return bitboard;
}

// Temporary version, need to use more fast
UNumspeed Bitboard::enabledCount(Type bitboard) {
    UNumspeed result = 0;

    while (bitboard != Bitboard::null) {
        auto const bit = bitScan(bitboard);
        bitboard ^= fromIndex(bit);

        ++result;
    }

    return result;
}

void Bitboard::initTables() {
    forRawYCoords(y)
    forRawXCoords(x) {
        auto coord = Coord::fromRaw(x, y);
        Type bitboard = makeUNum64(1) << (y|x);
        coordToBitboardTable[coord] = bitboard;
    }

    forRawYCoords(yFrom) 
    forRawXCoords(xFrom) 
    forRawYCoords(yTo) 
    forRawXCoords(xTo) {
        auto from = Coord::fromRaw(xFrom, yFrom);
        auto to = Coord::fromRaw(xTo, yTo);
        Type bitboard = coordToBitboardTable[from] ^ coordToBitboardTable[to];
        auto move = Move::create(from, to);

        moveToBitboardTable[move] = bitboard;
    }

    leftLine = Bitboard::fromCoord(Coord::fromString("a1"))
             | Bitboard::fromCoord(Coord::fromString("a2"))
             | Bitboard::fromCoord(Coord::fromString("a3"))
             | Bitboard::fromCoord(Coord::fromString("a4"))
             | Bitboard::fromCoord(Coord::fromString("a5"))
             | Bitboard::fromCoord(Coord::fromString("a6"))
             | Bitboard::fromCoord(Coord::fromString("a7"))
             | Bitboard::fromCoord(Coord::fromString("a8"));

    rightLine = Bitboard::fromCoord(Coord::fromString("h1"))
              | Bitboard::fromCoord(Coord::fromString("h2"))
              | Bitboard::fromCoord(Coord::fromString("h3"))
              | Bitboard::fromCoord(Coord::fromString("h4"))
              | Bitboard::fromCoord(Coord::fromString("h5"))
              | Bitboard::fromCoord(Coord::fromString("h6"))
              | Bitboard::fromCoord(Coord::fromString("h7"))
              | Bitboard::fromCoord(Coord::fromString("h8"));

    upLine = Bitboard::fromCoord(Coord::fromString("a8"))
           | Bitboard::fromCoord(Coord::fromString("b8"))
           | Bitboard::fromCoord(Coord::fromString("c8"))
           | Bitboard::fromCoord(Coord::fromString("d8"))
           | Bitboard::fromCoord(Coord::fromString("e8"))
           | Bitboard::fromCoord(Coord::fromString("f8"))
           | Bitboard::fromCoord(Coord::fromString("g8"))
           | Bitboard::fromCoord(Coord::fromString("h8"));


    downLine = Bitboard::fromCoord(Coord::fromString("a1"))
             | Bitboard::fromCoord(Coord::fromString("b1"))
             | Bitboard::fromCoord(Coord::fromString("c1"))
             | Bitboard::fromCoord(Coord::fromString("d1"))
             | Bitboard::fromCoord(Coord::fromString("e1"))
             | Bitboard::fromCoord(Coord::fromString("f1"))
             | Bitboard::fromCoord(Coord::fromString("g1"))
             | Bitboard::fromCoord(Coord::fromString("h1"));
}
