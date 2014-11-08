#include "uci.hpp"
#include "mover.hpp"
#include "bitboards.hpp"
#include "colors.hpp"
#include "pieces.hpp"
#include "generator.hpp"
#include "PST.hpp"
#include "score.hpp"
#include "zobrist.hpp"
#include "history.hpp"
#include "hash.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

void initTables() {
    Move::initTables();
    Bitboard::initTables();
    Color::initTables();
    Piece::initTables();
    Tables::initTables();
    Magic::initTables();
    PST::initTables();
    Score::initTables();
    Zobrist::initTables();
    History::flush();
    Hash::setTableSizeInMb(50);
}

int main(int, char**) {
    initTables();

    Uci::cycle();

    return 0;
}
