#include "generatorTables.hpp"

using namespace Tables;

Bitboard::Type Tables::knightMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::kingMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::bishopMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::rookMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::pawnCaptureMasks[makeUNumspeed(1) << Color::usedBitsReal][makeUNumspeed(1) << Coord::usedBits];

Bitboard::Type Tables::bishopFullMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type Tables::rookFullMasks[makeUNumspeed(1) << Coord::usedBits];

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

        for (Numspeed xDirection = -1; xDirection <= 1; xDirection += 2)
        for (Numspeed yDirection = -1; yDirection <= 1; yDirection += 2) {
            for (Numspeed delta = makeNumspeed(1); delta <= makeNumspeed(7); ++delta) {
                if (UNumspeed(x + delta*xDirection) <= makeUNumspeed(7) 
                 && UNumspeed(y + delta*yDirection) <= makeUNumspeed(7)) {
                    auto to = Coord::create(x + delta*xDirection, y + delta*yDirection);
                    toBits |= Bitboard::fromCoord(to);
                }
            }
        }

        bishopFullMasks[from] = toBits;
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

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xTo = 0; xTo <= 7; ++xTo) {
            const auto to = Coord::create(xTo, y);

            toBits |= Bitboard::fromCoord(to);
        }

        for (Numspeed yTo = 0; yTo <= 7; ++yTo) {
            const auto to = Coord::create(x, yTo);

            toBits |= Bitboard::fromCoord(to);
        }

        toBits &= ~Bitboard::fromCoord(from);

        rookFullMasks[from] = toBits;
    }

    forCoord(x)
    for(UNumspeed y = 0; y <= 7; ++y) {
        const auto from = Coord::create(x, y);
        auto whiteToBits = Bitboard::null;
        auto blackToBits = Bitboard::null;

        // Left capture
        if (x - 1 <= 7) {
            if (y+1 <= 7) {
                const auto whiteTo = Coord::create(x-1, y+1);
                whiteToBits |= Bitboard::fromCoord(whiteTo);
            }

            if (y-1 <= 7) {
                const auto blackTo = Coord::create(x-1, y-1);
                blackToBits |= Bitboard::fromCoord(blackTo);
            }
        }

        // Right capture
        if (x + 1 <= 7) {
            if (y+1 <= 7) {
                const auto whiteTo = Coord::create(x+1, y+1);
                whiteToBits |= Bitboard::fromCoord(whiteTo);
            }

            if (y-1 <= 7) {
                const auto blackTo = Coord::create(x+1, y-1);
                blackToBits |= Bitboard::fromCoord(blackTo);
            }
        }

        whiteToBits &= ~Bitboard::fromCoord(from);

        pawnCaptureMasks[White][from] = whiteToBits;
        pawnCaptureMasks[Black][from] = blackToBits;
    }

    pawnStartLine[White] = Bitboard::fromCoord(Coord::A2)
                         | Bitboard::fromCoord(Coord::B2)
                         | Bitboard::fromCoord(Coord::C2)
                         | Bitboard::fromCoord(Coord::D2)
                         | Bitboard::fromCoord(Coord::E2)
                         | Bitboard::fromCoord(Coord::F2)
                         | Bitboard::fromCoord(Coord::G2)
                         | Bitboard::fromCoord(Coord::H2);

    pawnStartLine[Black] = Bitboard::fromCoord(Coord::A7)
                         | Bitboard::fromCoord(Coord::B7)
                         | Bitboard::fromCoord(Coord::C7)
                         | Bitboard::fromCoord(Coord::D7)
                         | Bitboard::fromCoord(Coord::E7)
                         | Bitboard::fromCoord(Coord::F7)
                         | Bitboard::fromCoord(Coord::G7)
                         | Bitboard::fromCoord(Coord::H7);

    castleNeeded[White][King]  = Bitboard::fromCoord(Coord::F1)
                               | Bitboard::fromCoord(Coord::G1);

    castleNeeded[White][Queen] = Bitboard::fromCoord(Coord::D1)
                               | Bitboard::fromCoord(Coord::C1)
                               | Bitboard::fromCoord(Coord::B1);

    castleNeeded[Black][King]  = Bitboard::fromCoord(Coord::F8)
                               | Bitboard::fromCoord(Coord::G8);

    castleNeeded[Black][Queen] = Bitboard::fromCoord(Coord::D8)
                               | Bitboard::fromCoord(Coord::C8)
                               | Bitboard::fromCoord(Coord::B8);

    castleTarget[White][King] = Coord::G1;
    castleTarget[White][Queen] = Coord::C1;
    castleTarget[Black][King] = Coord::G8;
    castleTarget[Black][Queen] = Coord::C8;
}
