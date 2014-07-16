#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "moves.hpp"
#include "mover.hpp"
#include "generator.hpp"
#include "evaluation.hpp"
#include "scores.hpp"

namespace Search {
    template <Color::Type COLOR>
    Score::Type nodeSearch(Generator::MoveBuffer *moves, Board::Type &board, UNumspeed depth) {
        const auto OPP = COLOR == White? Black: White;

        if (depth == 0) {
            return Eval::material<COLOR>(board);
        }

        Score::Type maxScore = -Score::Infinity;

        Generator::forBoard<COLOR>(moves[depth], board);

        UNumspeed total = moves[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>(moves[depth][i], board)) {
                auto score = -nodeSearch<OPP>(moves, board, depth - 1);

                if (score > maxScore) {
                    score = maxScore;
                }
            }

            Move::unmake<COLOR>(moves[depth][i], board);
        }

        return maxScore;
    }

    template <Color::Type COLOR>
    Move::Type search(Board::Type &board, UNumspeed depth) {
        Generator::MoveBuffer *moves = new Generator::MoveBuffer[MAX_DEPTH];

        const auto OPP = COLOR == White? Black: White;

        Move::Type result = 0;
        Score::Type maxScore = -Score::Infinity;

        Generator::forBoard<COLOR>(moves[depth], board);

        UNumspeed total = moves[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>(moves[depth][i], board)) {
                auto score = -nodeSearch<OPP>(moves, board, depth - 1);

                if (score > maxScore) {
                    result = moves[depth][i];
                    score = maxScore;
                }
            }

            Move::unmake<COLOR>(moves[depth][i], board);
        }

        return result;
    }
}

#endif /* SEARCH_HPP */
