#ifndef KILLERS_HPP
#define KILLERS_HPP

#include "boards.hpp"

namespace Killer {
    inline Move::Type first(const Board::Type& board) {
        return (board.depthPtr)->killer1;
    }

    inline Move::Type second(const Board::Type& board) {
        return (board.depthPtr)->killer2;
    }

    inline void write(Move::Type move, Board::Type& board) {
        if (move == (board.depthPtr-1)->killer1) return;
        if (move == (board.depthPtr-1)->killer2) return;

        (board.depthPtr-1)->killer2 = (board.depthPtr-1)->killer1;
        (board.depthPtr-1)->killer1 = move;
    }
}

#endif /* KILLERS_HPP */
