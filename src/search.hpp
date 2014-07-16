#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "moves.hpp"
#include "mover.hpp"
#include "generator.hpp"
#include "evaluation.hpp"
#include "scores.hpp"

namespace Search {
    template <Color::Type COLOR>
    Score::Type nodeSearch(Move::Buffer *moves, Board::Type &board, UNumspeed depth, Score::Type alpha, Score::Type beta) {
        const auto OPP = COLOR == White? Black: White;

        if (depth == 0) {
            return Eval::material<COLOR>(board);
        }

        Generator::forBoard<COLOR>(moves[depth], board);

        UNumspeed total = moves[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>(moves[depth][i], board)) {
                auto score = -nodeSearch<OPP>(moves, board, depth - 1, -beta, -alpha);

                if (score > alpha) {
                    alpha = score;
                }
            }

            Move::unmake<COLOR>(moves[depth][i], board);

            if (alpha >= beta) {
                return beta;
            }
        }

        return alpha;
    }

    template <Color::Type COLOR>
    Move::Type search(Board::Type &board, UNumspeed depth) {
        Move::Buffer *moves = new Move::Buffer[MAX_DEPTH];

        const auto OPP = COLOR == White? Black: White;

        Move::Type result = 0;
        Score::Type alpha = -Score::Infinity;

        Generator::forBoard<COLOR>(moves[depth], board);

        UNumspeed total = moves[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>(moves[depth][i], board)) {
                auto score = -nodeSearch<OPP>(moves, board, depth - 1, -Score::Infinity, -alpha);

                if (score > alpha) {
                    result = moves[depth][i];
                    alpha = score;
                }
            }

            Move::unmake<COLOR>(moves[depth][i], board);
        }

        return result;
    }
}

#endif /* SEARCH_HPP */
