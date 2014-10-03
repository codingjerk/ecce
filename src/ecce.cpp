// @TODO, GLOBAL:
//    - Best UCI
//      - Support of info
//      - Support of go *
//      - Support of options
//      - Support of ponder
//      - Support of multipv

//    - Best Search
//      (+) - Write Simple Search (alpha-beta)
//      (+) - Extract PV
//      (+) - Write stop command
//      (+) - Incremental 
//      - Test search
//      - Negascout
//      - Sort LVA/MVV
//      - Move buffer in board
//      - Write quies
//      - Triple replies
//      - Hash table

//    - Best Eval
//      (+) - Material
//      (+) - PST
//      - Some other positional factors
//      - Test Eval

//    - Best Time Manager
//      (+) - Write Simple TM
//      (+) - Use depth in TM
//      - Use time in TM
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
