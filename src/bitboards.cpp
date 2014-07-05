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

    for (Numspeed y = Coord::componentHighter; y >= Numspeed(Coord::componentLower); --y) {
        for (UNumspeed x = Coord::componentLower; x <= Coord::componentHighter; ++x) {
            result += '0' + bool(bitboard & fromCoord(Coord::create(x, y)));
        }

        if (y != Coord::componentLower) result += '\n';
    }

    return result;
}

void Bitboard::initTables() {
    for (UNumspeed y = Coord::componentLower; y <= Coord::componentHighter; ++y)
    for (UNumspeed x = Coord::componentLower; x <= Coord::componentHighter; ++x) {
        auto coord = Coord::create(x, y);
        Type bitboard = makeUNum64(1) << (y * makeUNum64(8) + x);
        coordToBitboardTable[coord] = bitboard;
    }

    for (UNumspeed yFrom = Coord::componentLower; yFrom <= Coord::componentHighter; ++yFrom) 
    for (UNumspeed xFrom = Coord::componentLower; xFrom <= Coord::componentHighter; ++xFrom) 
    for (UNumspeed yTo = Coord::componentLower; yTo <= Coord::componentHighter; ++yTo) 
    for (UNumspeed xTo = Coord::componentLower; xTo <= Coord::componentHighter; ++xTo) {
        auto from = Coord::create(xFrom, yFrom);
        auto to = Coord::create(xTo, yTo);
        Type bitboard = coordToBitboardTable[from] ^ coordToBitboardTable[to];
        auto move = Move::create(from, to);

        moveToBitboardTable[move] = bitboard;
    }
}
