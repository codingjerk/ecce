#include "bitboards.hpp"
#include "cstf.hpp"

using namespace Bitboard;

Type coordToBitboardTable[makeUNumspeed(1) << Coord::usedBits];
Type moveToBitboardTable[makeUNumspeed(1) << Move::usedBits];

Type Bitboard::fromCoord(const Coord::Type coord) {
    ASSERT((coord & Coord::typeMask) == coord);

    return coordToBitboardTable[coord];
}

Type Bitboard::fromIndex(const UNumspeed index) {
    ASSERT(index < 64);

    return 1ull << index;
}

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

// BitScan function from https://chessprogramming.wikispaces.com/BitScan
// @Author: Gerd Isenberg
union {
    double d;
    struct {
        unsigned int _1       : 32;
        unsigned int _2       : 20;
        unsigned int exponent : 11;
        unsigned int _3       : 1;
    };
} bitScanStuct;

UNumspeed Bitboard::bitScan(Type bitboard) {
   bitScanStuct.d = (double)(bitboard & -bitboard);
   return bitScanStuct.exponent - 1023;
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
}
