#include "generatorTables.hpp"

using namespace Tables;

Bitboard::Type Tables::knightMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::kingMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::bishopMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::rookMasks[makeUNumspeed(1) << Coord::usedBits];

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
}
