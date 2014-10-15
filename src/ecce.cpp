// @TODO, GLOBAL:
//    - Best UCI
//      - Support of options

//    - Best Search
//      - Negascout
//      - Hash table

//    - Best Eval

//    - Best Time Manager

//    - Best Speed
//      - Optimize algo
//      - Optimize razoring
//      - Optimize evaluation
//      - Optimize parallel

#include "uci.hpp"
#include "mover.hpp"
#include "bitboards.hpp"
#include "colors.hpp"
#include "pieces.hpp"
#include "generator.hpp"
#include "PST.hpp"
#include "score.hpp"
#include "zobrist.hpp"

void initTables() {
    Move::initTables();
    Bitboard::initTables();
    Color::initTables();
    Piece::initTables();
    Generator::initTables();
    PST::initTables();
    Score::initTables();
    Zobrist::initTables();
}

int main(int, char**) {
    initTables();

    Uci::cycle();

    return 0;
}
