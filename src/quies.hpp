#ifndef QUIES_HPP
#define QUIES_HPP

#include <iostream>

#include "eval.hpp"
#include "boards.hpp"
#include "mover.hpp"
#include "generatorPhases.hpp"
#include "moves.hpp"
#include "statistic.hpp"

namespace Search {
    template <Color::Type COLOR>
    Score::Type quiesce(Board::Type &board, Score::Type alpha, Score::Type beta) {
        Statistic::increaseQuiesceNodes();

        MAKEOPP(COLOR);

		if (Board::isRepeat(board)) return Score::Draw;
        if (Board::isFifty(board))  return Score::Draw;

        alpha = max(alpha, Eval::total<COLOR>(board));
        if(alpha >= beta) return beta;
        
        Captures::phase<COLOR>(Board::currentBuffer(board), board);
        UNumspeed total = Board::currentBuffer(board)[0];
        Move::Type move;
        Score::Type score;
        bool noMoves = true;
        for (UNumspeed i = 1; i <= total; ++i) {
            move = Board::currentBuffer(board)[i];

            if (Move::makeCapture<COLOR>(move, board)) {
                score = -quiesce<OPP>(board, -beta, -alpha);
                noMoves = false;
                if (score > alpha) {
                    Statistic::quiesceAlphaUpped();
                    alpha = score;
                } else {
                    Statistic::quiesceAlphaPruned();
                }
            }

            Move::unmakeCapture<COLOR>(move, board);

            if (alpha >= beta) {
                Statistic::quiesceBetaPruned();
                break;
            }
        }

        if (noMoves) {
            Statistic::noQuiesceMoves();
        }

        return alpha;
    }
}

#endif /* QUIES_HPP */
