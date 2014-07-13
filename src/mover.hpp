#ifndef MOVER_HPP
#define MOVER_HPP

#include "core.hpp"

#include "moves.hpp"
#include "boards.hpp"

namespace Move {
    extern Boolspeed (*specialMake[8])(Move::Type, Board::Type&);
    extern void (*specialUnmake[8])(Move::Type, Board::Type&);

    void initTables();

    //@TODO(FAST): Make it template
    inline Boolspeed make(Type move, Board::Type& board) {
        const auto specialIndex = Move::special(move);
        return specialMake[specialIndex](move, board);    
    }

    inline void unmake(Type move, Board::Type& board) {
        const auto specialIndex = Move::special(move);
        specialUnmake[specialIndex](move, board);
    }
}

#endif /* MOVER_HPP */
