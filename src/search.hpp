#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "tm.hpp"
#include "moves.hpp"
#include "checker.hpp"
#include "score.hpp"
#include "eval.hpp"

namespace Search {
	// @TODO: Use COLOR as Color::Type in all templates
	Move::Type bm;

	template <Color::Type COLOR, bool ROOT>
	Score::Type alphaBeta(Move::Buffer *buffer, Board::Type &board, Score::Type alpha, Score::Type beta, Numspeed depth) {
        MAKEOPP(COLOR);
		if (Checker::isCheck<OPP>(board)) return -Score::Infinity + MAX_DEPTH - depth;

		if (depth <= 0) return Eval::total<COLOR>(board);

        Generator::forBoard<COLOR>(buffer[depth], board);
        UNumspeed total = buffer[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
			if (Move::make<COLOR>(buffer[depth][i], board)) {
				auto score = -alphaBeta<OPP, false>(buffer, board, -beta, -alpha, depth - 1);

				if (score > alpha) {
					alpha = score;
					if (ROOT) bm = buffer[depth][i];
				}

				if (ROOT) std::cout << Move::show(buffer[depth][i]) << ": " << score << "\n";
			}

            Move::unmake<COLOR>(buffer[depth][i], board);

			if (alpha >= beta) break;
        }

		return alpha;
	}

	template <Color::Type COLOR>
	Move::Type start(Board::Type &board, TM::DepthLimit depth) {
		Move::Buffer *moves = new Move::Buffer[MAX_DEPTH];
		auto score = alphaBeta<COLOR, true>(moves, board, -Score::Infinity, Score::Infinity, depth.maxDepth);
		delete moves;
		return bm;
	}

	Move::Type start(Board::Type &board, TM::DepthLimit depth) {
        if (board.turn == White) {
            return start<White>(board, depth);
        } else {
            return start<Black>(board, depth);
        }
	}
}

#endif /* SEARCH_HPP */
