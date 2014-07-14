#include "bitboards.hpp"
#include "cstf.hpp"

using namespace Bitboard;

Type Bitboard::upLine;
Type Bitboard::downLine;
Type Bitboard::leftLine;
Type Bitboard::rightLine;

Type coordToBitboardTable[makeUNumspeed(1) << Coord::usedBits];

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

void Bitboard::initTables() {
    forRawYCoords(y)
    forRawXCoords(x) {
        auto coord = Coord::fromRaw(x, y);
        Type bitboard = makeUNum64(1) << (y|x);
        coordToBitboardTable[coord] = bitboard;
    }

    leftLine = Bitboard::fromCoord(Coord::A1)
             | Bitboard::fromCoord(Coord::A2)
             | Bitboard::fromCoord(Coord::A3)
             | Bitboard::fromCoord(Coord::A4)
             | Bitboard::fromCoord(Coord::A5)
             | Bitboard::fromCoord(Coord::A6)
             | Bitboard::fromCoord(Coord::A7)
             | Bitboard::fromCoord(Coord::A8);

    rightLine = Bitboard::fromCoord(Coord::H1)
              | Bitboard::fromCoord(Coord::H2)
              | Bitboard::fromCoord(Coord::H3)
              | Bitboard::fromCoord(Coord::H4)
              | Bitboard::fromCoord(Coord::H5)
              | Bitboard::fromCoord(Coord::H6)
              | Bitboard::fromCoord(Coord::H7)
              | Bitboard::fromCoord(Coord::H8);

    upLine = Bitboard::fromCoord(Coord::A8)
           | Bitboard::fromCoord(Coord::B8)
           | Bitboard::fromCoord(Coord::C8)
           | Bitboard::fromCoord(Coord::D8)
           | Bitboard::fromCoord(Coord::E8)
           | Bitboard::fromCoord(Coord::F8)
           | Bitboard::fromCoord(Coord::G8)
           | Bitboard::fromCoord(Coord::H8);


    downLine = Bitboard::fromCoord(Coord::A1)
             | Bitboard::fromCoord(Coord::B1)
             | Bitboard::fromCoord(Coord::C1)
             | Bitboard::fromCoord(Coord::D1)
             | Bitboard::fromCoord(Coord::E1)
             | Bitboard::fromCoord(Coord::F1)
             | Bitboard::fromCoord(Coord::G1)
             | Bitboard::fromCoord(Coord::H1);
}
