// @TODO, GLOBAL:
//    - Best UCI
//      - Support of go *
//      - Support of options
//      - Support of ponder
//      - Support of multipv

//    - Best Search
//      - Write Simple Search (alpha-beta)
//      - Extract PV
//      - Write stop command
//      - Test search

//    - Best Eval
//      - Test Eval

//    - Best Time Manager
//      - Write Simple TM
//      - Allow to use go *
//      - Test TM

//    - Best Speed
//      - Test current speed
//      - Optimize low
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

int main(int, char**) {
    Move::initTables();
    Bitboard::initTables();
    Color::initTables();
    Piece::initTables();
    Generator::initTables();
    PST::initTables();

    Uci::cycle();

    return 0;
}
