#include "generatorTables.hpp"

using namespace Tables;

Bitboard::Type Tables::knightMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::kingMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::bishopMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::rookMasks[makeUNumspeed(1) << Coord::usedBits];

Bitboard::Type Tables::pawnStartLine[makeUNumspeed(1) << Color::usedBitsReal];

Bitboard::Type Tables::castleNeeded[makeUNumspeed(1) << Color::usedBitsReal][makeUNumspeed(1) << Dignity::usedBits];
Coord::Type Tables::castleTarget[makeUNumspeed(1) << Color::usedBitsReal][makeUNumspeed(1) << Dignity::usedBits];

void Tables::initTables() {
    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);

        auto toBits = Bitboard::null;
        for (Numspeed xDelta = -2; xDelta <= 2; ++xDelta)
        for (Numspeed yDelta = -2; yDelta <= 2; ++yDelta) {
            if (xDelta == 0 || yDelta == 0 || std::abs(xDelta) == std::abs(yDelta)) continue;

            if ((x + xDelta <= 7ull) && (y + yDelta <= 7ull)) {
                toBits |= Bitboard::fromCoord(Coord::create(x + xDelta, y + yDelta));
            }
        }

        knightMasks[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xDelta = -1; xDelta <= 1; ++xDelta)
        for (Numspeed yDelta = -1; yDelta <= 1; ++yDelta) {
            if (xDelta == 0 && yDelta == 0) continue;

            if ((x + xDelta <= 7ull) && (y + yDelta <= 7ull)) {
                toBits |= Bitboard::fromCoord(Coord::create(x + xDelta, y + yDelta));
            }
        }

        kingMasks[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xDirection = -1; xDirection <= 1; xDirection += 2)
        for (Numspeed yDirection = -1; yDirection <= 1; yDirection += 2) {
            for (Numspeed delta = makeNumspeed(1); delta <= makeNumspeed(6); ++delta) {
                if (UNumspeed(x + delta*xDirection) <= makeUNumspeed(6) && UNumspeed(y + delta*yDirection) <= makeUNumspeed(6)
                 && UNumspeed(x + delta*xDirection) >= makeUNumspeed(1) && UNumspeed(y + delta*yDirection) >= makeUNumspeed(1)) {
                    auto to = Coord::create(x + delta*xDirection, y + delta*yDirection);
                    toBits |= Bitboard::fromCoord(to);
                }
            }
        }

        bishopMasks[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xTo = 1; xTo <= 6; ++xTo) {
            const auto to = Coord::create(xTo, y);

            toBits |= Bitboard::fromCoord(to);
        }

        for (Numspeed yTo = 1; yTo <= 6; ++yTo) {
            const auto to = Coord::create(x, yTo);

            toBits |= Bitboard::fromCoord(to);
        }

        toBits &= ~Bitboard::fromCoord(from);

        rookMasks[from] = toBits;
    }

    pawnStartLine[White] = Bitboard::fromCoord(Coord::fromString("a2"))
                         | Bitboard::fromCoord(Coord::fromString("b2"))
                         | Bitboard::fromCoord(Coord::fromString("c2"))
                         | Bitboard::fromCoord(Coord::fromString("d2"))
                         | Bitboard::fromCoord(Coord::fromString("e2"))
                         | Bitboard::fromCoord(Coord::fromString("f2"))
                         | Bitboard::fromCoord(Coord::fromString("g2"))
                         | Bitboard::fromCoord(Coord::fromString("h2"));

    pawnStartLine[Black] = Bitboard::fromCoord(Coord::fromString("a7"))
                         | Bitboard::fromCoord(Coord::fromString("b7"))
                         | Bitboard::fromCoord(Coord::fromString("c7"))
                         | Bitboard::fromCoord(Coord::fromString("d7"))
                         | Bitboard::fromCoord(Coord::fromString("e7"))
                         | Bitboard::fromCoord(Coord::fromString("f7"))
                         | Bitboard::fromCoord(Coord::fromString("g7"))
                         | Bitboard::fromCoord(Coord::fromString("h7"));

    castleNeeded[White][King]  = Bitboard::fromCoord(Coord::fromString("f1"))
                               | Bitboard::fromCoord(Coord::fromString("g1"));

    castleNeeded[White][Queen] = Bitboard::fromCoord(Coord::fromString("d1"))
                               | Bitboard::fromCoord(Coord::fromString("c1"))
                               | Bitboard::fromCoord(Coord::fromString("b1"));

    castleNeeded[Black][King]  = Bitboard::fromCoord(Coord::fromString("f8"))
                               | Bitboard::fromCoord(Coord::fromString("g8"));

    castleNeeded[Black][Queen] = Bitboard::fromCoord(Coord::fromString("d8"))
                               | Bitboard::fromCoord(Coord::fromString("c8"))
                               | Bitboard::fromCoord(Coord::fromString("b8"));

    castleTarget[White][King] = Coord::fromString("g1");
    castleTarget[White][Queen] = Coord::fromString("c1");
    castleTarget[Black][King] = Coord::fromString("g8");
    castleTarget[Black][Queen] = Coord::fromString("c8");
}
