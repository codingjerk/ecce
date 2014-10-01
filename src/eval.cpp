#include "eval.hpp"

#include "dignities.hpp"
#include "colors.hpp"

using namespace Eval;

Score::Type Eval::PSTTable[Piece::usedBits][Coord::usedBits];

void Eval::initTables() {
    forCoord (x)
    forCoord (y)
    forDignities(d)
    forColors(c) {
        const auto coord = Coord::create(x, y);
        const auto piece = Piece::create(c, d);

        PSTTable[piece][coord] = Score::Null;
    }
}
