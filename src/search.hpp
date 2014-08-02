#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "core.hpp"
#include "moves.hpp"
#include "mover.hpp"
#include "scores.hpp"
#include "colors.hpp"
#include "generator.hpp"
#include "boards.hpp"
#include "evaluation.hpp"
#include "checker.hpp"

namespace Search {
    template <Color::Type COLOR>
    Score::Type node(Move::Buffer *moves, UNumspeed depth, Score::Type alpha, Score::Type beta) {
        const auto OPP = COLOR == White? Black: White;

        // @TODO: Return mate in score
        if (Checker::isCheck<OPP>(Board::master)) 
            return Score::Infinity;

        if (depth == 0) {
            return Eval::material<COLOR>(Board::master);
        }

        Score::Type bestscore = -Score::Infinity;

        Generator::forBoard<COLOR>(*moves, Board::master);

        UNumspeed total = (*moves)[0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>((*moves)[i], Board::master)) {
                auto score = -Search::node<OPP>(moves + 1, depth - 1, -beta, -alpha);

                if (score > bestscore) {
                    bestscore = score;
                }
            }

            Move::unmake<COLOR>((*moves)[i], Board::master);

            if (bestscore > alpha) {
                alpha = bestscore;
            }

            if (alpha >= beta) {
                return alpha;
            }
        }

        return bestscore;
    }

    template <Color::Type COLOR>
    Move::Type bestmove(UNumspeed depth) {
        const auto OPP = COLOR == White? Black: White;

        Move::Type bestmove;
        Score::Type bestscore = -Score::Infinity;

        Move::Buffer *moves = new Move::Buffer[100];
        Generator::forBoard<COLOR>(*moves, Board::master);

        UNumspeed total = (*moves)[0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>((*moves)[i], Board::master)) {
                auto score = -Search::node<OPP>(moves + 1, depth - 1, -Score::Infinity, -bestscore);

                if (score > bestscore) {
                    bestscore = score;
                    bestmove = (*moves)[i];
                }
            }

            Move::unmake<COLOR>((*moves)[i], Board::master);
        }

        delete[] moves;

        std::cout << "info depth " << depth
                  << " pv " << Move::show(bestmove)
                  << " score " << bestscore
                  << "\n" << std::flush;

        return bestmove;
    }

    Move::Type bestmove(UNumspeed depth) {
        if (Board::master.turn == White) {
            return bestmove<White>(depth);
        } else {
            return bestmove<Black>(depth);
        }
    }

    void incremental(UNumspeed maxDepth) {
        Move::Type bestmove;

        for (UNumspeed depth = 1; depth <= maxDepth; ++depth) {
            bestmove = Search::bestmove(depth);
        }

        std::cout << "bestmove " << Move::show(bestmove) << "\n" << std::flush;
    }
}

#endif /* SEARCH_HPP */
