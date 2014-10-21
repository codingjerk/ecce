// @TODO, GLOBAL:
//    - Best UCI
//      - Support of options

//    - Best Search
//      - Negascout
//      - Null move
//      - Hash table

//    - Best Eval
//      - Use genetic algos to optimize evaluation factors
//      - Use some hard-to-calc positional factorors as mobility

//    - Best Time Manager
//      - Increase or decrease time by static position score
//      - Increase or decrease time by dynamic delta score of last searches
//      - Every new depth checks
//      - Use some dirty hacks to cheating with time

//    - Best Speed
//      - Optimize generation
//      - Optimize razoring
//      - Optimize evaluation

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

void initTables() {
    Move::initTables();
    Bitboard::initTables();
    Color::initTables();
    Piece::initTables();
    Generator::initTables();
    PST::initTables();
    Score::initTables();
    Zobrist::initTables();
    History::flush();
}

int main(int, char**) {
    initTables();

    Uci::cycle();

    return 0;
}
