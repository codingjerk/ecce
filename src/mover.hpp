#ifndef MOVER_HPP
#define MOVER_HPP

#include "core.hpp"

#include "moves.hpp"
#include "boards.hpp"

namespace Move {
    extern Boolspeed (*specialMakeWhite[6])(Move::Type, Board::Type&);
    extern Boolspeed (*specialMakeBlack[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeWhite[6])(Move::Type, Board::Type&);
    extern void (*specialUnmakeBlack[6])(Move::Type, Board::Type&);

    void initTables();

    template <Color::Type COLOR> 
    inline Boolspeed make(Type, Board::Type&);

    template <>
    inline Boolspeed make<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeWhite[specialIndex](move, board);    
    }

    template <>
    inline Boolspeed make<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        return specialMakeBlack[specialIndex](move, board);    
    }

    template <Color::Type COLOR>
    inline void unmake(Type, Board::Type&);

    template <>
    inline void unmake<White>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeWhite[specialIndex](move, board);
    }

    template <>
    inline void unmake<Black>(Type move, Board::Type& board) {
        const auto specialIndex = special(move);
        specialUnmakeBlack[specialIndex](move, board);
    }

    inline Boolspeed make(Type move, Board::Type& board) {
        if (board.turn == White) {
            return make<White>(move, board);
        } else {
            return make<Black>(move, board);
        }
    }

    inline void unmake(Type move, Board::Type& board) {
        if (board.turn == White) {
            unmake<White>(move, board);
        } else {
            unmake<Black>(move, board);
        }
    }
}

#endif /* MOVER_HPP */
