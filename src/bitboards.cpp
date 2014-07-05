#include "bitboards.hpp"
#include "cstf.hpp"

using namespace Bitboard;

Type coordToBitboardTable[makeUNumspeed(1) << Coord::usedBits];
Type moveToBitboardTable[makeUNumspeed(1) << Move::usedBits];

Type Bitboard::fromCoord(const Coord::Type coord) {
    ASSERT((coord & Coord::typeMask) == coord);

    return coordToBitboardTable[coord];
}

Type Bitboard::fromMove(const Move::Type move) {
    ASSERT((move & Move::typeMask) == move);

    return moveToBitboardTable[move];
}

std::string Bitboard::show(const Type bitboard) {
    std::string result;
    result.reserve(64+8);

    for (Numspeed y = 8-1; y >= 0; --y) {
        for (UNumspeed x = 0; x < 8; ++x) {
            result += '0' + bool(bitboard & fromCoord(Coord::create(x, y)));
        }

        result += '\n';
    }

    return result;
}

void Bitboard::initTables() {
    for (UNumspeed y = 0; y < 8; ++y)
    for (UNumspeed x = 0; x < 8; ++x) {
        auto coord = Coord::create(x, y);
        Type bitboard = makeUNum64(1) << (y * makeUNum64(8) + x);
        coordToBitboardTable[coord] = bitboard;
    }

    for (UNumspeed yFrom = 0; yFrom < 8; ++yFrom) 
    for (UNumspeed xFrom = 0; xFrom < 8; ++xFrom) 
    for (UNumspeed yTo = 0; yTo < 8; ++yTo) 
    for (UNumspeed xTo = 0; xTo < 8; ++xTo) {
        auto from = Coord::create(xFrom, yFrom);
        auto to = Coord::create(xTo, yTo);
        Type bitboard = coordToBitboardTable[from] ^ coordToBitboardTable[to];
        auto move = Move::create(from, to);

        moveToBitboardTable[move] = bitboard;
    }
}
