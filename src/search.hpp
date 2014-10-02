#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <vector>

#include "tm.hpp"
#include "moves.hpp"
#include "checker.hpp"
#include "score.hpp"
#include "eval.hpp"

#ifdef OSLINUX
	#include <unistd.h>
#else
	#include <Windows.h>
#endif

namespace Search {
	using PV = std::vector<Move::Type>;

	bool stopSearch;

	// @TODO: Use COLOR as Color::Type in all templates
	template <Color::Type COLOR>
	Score::Type alphaBeta(Move::Buffer *buffer, Board::Type &board, Score::Type alpha, Score::Type beta, Numspeed depth, PV &pv) {
        MAKEOPP(COLOR);
		if (Checker::isCheck<OPP>(board)) return -Score::Infinity + MAX_DEPTH - depth;

		if (depth <= 0) return Eval::total<COLOR>(board);

		if (stopSearch) {
			return -Score::Infinity; 
		} else if (isSearchInterrupted()) {
			stopSearch = true;
		}

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
	Move::Type simple(Board::Type &board, TM::DepthLimit depth) {
		stopSearch = false;

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

	Move::Type simple(Board::Type &board, TM::DepthLimit depth) {
        if (board.turn == White) {
            return simple<White>(board, depth);
        } else {
            return simple<Black>(board, depth);
        }
	}

	bool isInputAvailable() {
	#ifdef OSLINUX
		int val;
		fd_set set[1];
		struct timeval time_val[1];

		FD_ZERO(set);
		FD_SET(STDIN_FILENO,set);

		time_val->tv_sec = 0;
		time_val->tv_usec = 0;

		val = select(STDIN_FILENO+1,set,NULL,NULL,time_val);

		return val > 0;
	#else
		static bool init = false, is_pipe;
		static HANDLE stdin_h;
		DWORD val, error;

		if (stdin->_cnt > 0) return true;

		if (!init) {
			init = true;
			stdin_h = GetStdHandle(STD_INPUT_HANDLE);
			is_pipe = !GetConsoleMode(stdin_h, &val); 

			if (!is_pipe) {
				SetConsoleMode(stdin_h, val & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
				FlushConsoleInputBuffer(stdin_h); 
			}
		}

		if (is_pipe) {
			if (!PeekNamedPipe(stdin_h,NULL,0,NULL,&val ,NULL)) {
				return true;
			}
			return val > 0;
		} else {
			GetNumberOfConsoleInputEvents(stdin_h,&val);
			return val > 1;
		}

		return false;
	#endif
	}  

	bool isSearchInterrupted()
	{
		if(isInputAvailable())
		{
			std::string input;
			std::getline(std::cin, input);
			return (input == "stop");
		}

		return false;
	}
}

#endif /* SEARCH_HPP */
