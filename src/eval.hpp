#ifndef EVAL_HPP
#define EVAL_HPP

#include "score.hpp"
#include "boards.hpp"
#include "colors.hpp"

namespace Eval {
    template <Color::Type WHO> 
    inline Score::Type material(const Board::Type& board) {
        if (WHO == White) {
            return board.materialScore;
        } else {
            return -board.materialScore;
        }
    }
    
    template <Color::Type WHO> 
    inline Score::Type positional(const Board::Type &board) {
        if (WHO == White) {
            return board.positionalScore;
        } else {
            return -board.positionalScore;
        }
    }
    
    template <Color::Type WHO>
    inline Score::Type total(const Board::Type &board) {
        if (WHO == White) {
            return board.materialScore + board.positionalScore;
        } else {
            return -(board.materialScore + board.positionalScore);
        }
    }
}

#endif /* EVAL_HPP */
