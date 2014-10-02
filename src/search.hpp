#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <vector>

#include "tm.hpp"
#include "moves.hpp"
#include "checker.hpp"
#include "score.hpp"
#include "eval.hpp"

namespace Search {
	using PV = std::vector<Move::Type>;

	// @TODO: Use COLOR as Color::Type in all templates
	template <Color::Type COLOR>
	Score::Type alphaBeta(Move::Buffer *buffer, Board::Type &board, Score::Type alpha, Score::Type beta, Numspeed depth, PV &pv) {
        MAKEOPP(COLOR);
		if (Checker::isCheck<OPP>(board)) return -Score::Infinity + MAX_DEPTH - depth;

		if (depth <= 0) return Eval::total<COLOR>(board);

        Generator::forBoard<COLOR>(buffer[depth], board);
        UNumspeed total = buffer[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
			std::vector<Move::Type> childPV;

			if (Move::make<COLOR>(buffer[depth][i], board)) {
				auto score = -alphaBeta<OPP>(buffer, board, -beta, -alpha, depth - 1, childPV);

				if (score > alpha) {
					alpha = score;
					pv.clear();  
					pv.push_back(buffer[depth][i]);  
					std::copy(childPV.begin(), childPV.end(), back_inserter(pv));
				}
			}

            Move::unmake<COLOR>(buffer[depth][i], board);

			if (alpha >= beta) break;
        }

		return alpha;
	}

	template <Color::Type COLOR>
	Move::Type start(Board::Type &board, TM::DepthLimit depth) {
		Move::Buffer *moves = new Move::Buffer[MAX_DEPTH];
		PV pv;
		auto score = alphaBeta<COLOR>(moves, board, -Score::Infinity, Score::Infinity, depth.maxDepth, pv);
		delete moves;

		std::cout << "PV: ";
		for (auto move: pv) {
			std::cout << Move::show(move) << " ";
		}
		std::cout << "\n";

		if (!pv.empty()) {
			return pv[0];
		} else {
			return Move::create(Coord::A1, Coord::A1, Piece::null);
		}
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
