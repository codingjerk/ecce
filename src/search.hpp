#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "tm.hpp"

namespace Search {
	void alphaBeta(Move::Buffer *buffer, Board::Type &board, Numspeed depth) {

	}

	template <Color::Type COLOR>
	void start(Board::Type &board, TM::DepthLimit depth) {
		
	}

	void start(Board::Type &board, TM::DepthLimit depth) {
        if (board.turn == White) {
            return start<White>(board, depth);
        } else {
            return start<Black>(board, depth);
        }
	}
}

#endif /* SEARCH_HPP */
