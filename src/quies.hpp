#ifndef QUIES_HPP
#define QUIES_HPP

#include "eval.hpp"
#include "boards.hpp"
#include "mover.hpp"
#include "generatorPhases.hpp"
#include "moves.hpp"
#include "statistic.hpp"

namespace Search {
    template <Color::Type COLOR>
    Score::Type quiesce(Board::Type &board, Score::Type alpha, Score::Type beta) {
        ++totalNodes;
        Statistic::increaseQuiesceNodes();

        MAKEOPP(COLOR);

        alpha = max(alpha, Eval::total<COLOR>(board));
        if (alpha >= beta) return alpha;

        Move::Type move;
        Score::Type score;
        bool noMoves = true;
        forQuiescePhases(phase, Generator::quiescePhases<COLOR>()) {
            phase.generate(Board::buffer(board), board);
            UNumspeed total = Board::buffer(board)[0];
            for (UNumspeed i = 1; i <= total; ++i) {
                move = Board::buffer(board)[i];

                if (phase.make(move, board)) {
                    score = -quiesce<OPP>(board, -beta, -alpha);
                    noMoves = false;
                    if (score > alpha) {
                        Statistic::quiesceAlphaUpped();
                        alpha = score;
                    }
                    else {
                        Statistic::quiesceAlphaPruned();
                    }
                }

                phase.unmake(move, board);

                if (alpha >= beta) {
                    Statistic::quiesceBetaPruned();
                    break;
                }
            }
        }

        if (noMoves) {
            Statistic::noQuiesceMoves();
        }

        return alpha;
    }
}

#endif /* QUIES_HPP */
