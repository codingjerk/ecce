// @TODO, GLOBAL:
//    - Best UCI
//      - Support of info (seldepth (extract from board), matescore
//      - Support of options

//    - Best Search
//      - Test search
//      - Negascout
//      - Sort LVA/MVV
//      - Write quies
//      - Triple replies
//      - Hash table

//    - Best Eval
//      - Some other positional factors
//      - Test Eval

//    - Best Time Manager
//      - Test TM

//    - Best Speed
//      - Test current speed
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
