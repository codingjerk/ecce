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
// 7.931 total
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

/*
// Bitscan by Modulo
// 6.341 total
UNumspeed Bitboard::bitScan(Type bitboard) {
   static const int lookup67[67+1] = {
      64,  0,  1, 39,  2, 15, 40, 23,
       3, 12, 16, 59, 41, 19, 24, 54,
       4, -1, 13, 10, 17, 62, 60, 28,
      42, 30, 20, 51, 25, 44, 55, 47,
       5, 32, -1, 38, 14, 22, 11, 58,
      18, 53, 63,  9, 61, 27, 29, 50,
      43, 46, 31, 37, 21, 57, 52,  8,
      26, 49, 45, 36, 56,  7, 48, 35,
       6, 34, 33, -1 };
   return lookup67[(bitboard & -bitboard) % 67];
}*/

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
