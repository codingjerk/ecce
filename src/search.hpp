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
	typedef bool (*Interupter)();

    bool stopSearch;
	UNumspeed totalNodes;
	UNumspeed endTime;

    template <Color::Type COLOR, Interupter isInterupt>
    Score::Type alphaBeta(Board::Type &board, Score::Type alpha, Score::Type beta, Numspeed depth, PV &pv) {
		++totalNodes;
        MAKEOPP(COLOR);
        if (Checker::isCheck<OPP>(board)) return Score::Infinity - MAX_DEPTH + Board::ply(board);

        if (depth <= 0) return Eval::total<COLOR>(board);

		if (stopSearch || isInterupt()) {
			stopSearch = true;
			return 0;
		}

		Generator::forBoard<COLOR>(Board::currentBuffer(board), board);
        UNumspeed total = Board::currentBuffer(board)[0];
        for (UNumspeed i = 1; i <= total; ++i) {
            std::vector<Move::Type> childPV;
			const Move::Type move = Board::currentBuffer(board)[i];

            if (Move::make<COLOR>(move, board)) {
                auto score = -alphaBeta<OPP, isInterupt>(board, -beta, -alpha, depth - 1, childPV);

                if (score > alpha) {
                    alpha = score;
                    pv.clear();  
                    pv.push_back(move);  
                    std::copy(childPV.begin(), childPV.end(), back_inserter(pv));
                }
            }

            Move::unmake<COLOR>(move, board);

            if (alpha >= beta) break;
        }

        return alpha;
    }

    std::string showPV(PV &pv) {
        std::string result;

        for (auto move: pv) {
            result += Move::show(move) + " ";
        }

        return result;
    }

    template <Color::Type COLOR>
    Move::Type simple(Board::Type &board, TM::DepthLimit depth) {
        stopSearch = false;

        PV pv;
		auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth.maxDepth, pv);

        std::cout << "info depth " << depth.maxDepth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << showPV(pv) << "\n" << std::flush; 

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

    template <Color::Type COLOR>
    Move::Type incremental(Board::Type &board, TM::DepthLimit depthLimit) {
        stopSearch = false;

        Move::Type bestMove = Move::create(Coord::A1, Coord::A1, Piece::null);

        for (Numspeed depth = 1; depth <= depthLimit.maxDepth; ++depth) {
            PV pv;
			totalNodes = 0;
            auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth, pv);
        
            if (stopSearch) break;

			std::cout << "info depth " << depth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << showPV(pv) << "\n" << std::flush;

            if (!pv.empty()) {
                bestMove = pv[0];
            }
        }

        return bestMove;
    }

    Move::Type incremental(Board::Type &board, TM::DepthLimit depth) {
        if (board.turn == White) {
            return incremental<White>(board, depth);
        } else {
            return incremental<Black>(board, depth);
        }
    }

    template <Color::Type COLOR>
    Move::Type incremental(Board::Type &board, TM::TimeLimit timeLimit) {
        stopSearch = false;

        Move::Type bestMove = Move::create(Coord::A1, Coord::A1, Piece::null);

		endTime = GetTickCount() + timeLimit.maxTime;
        for (Numspeed depth = 1; depth <= MAX_DEPTH; ++depth) {
            PV pv;
			totalNodes = 0;
            auto score = alphaBeta<COLOR, timeInterupter>(board, -Score::Infinity, Score::Infinity, depth, pv);
        
            if (stopSearch) break;

			std::cout << "info depth " << depth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << showPV(pv) << "\n" << std::flush;

            if (!pv.empty()) {
                bestMove = pv[0];
            }
        }

        return bestMove;
    }

    Move::Type incremental(Board::Type &board, TM::TimeLimit timeLimit) {
        if (board.turn == White) {
            return incremental<White>(board, timeLimit);
        } else {
            return incremental<Black>(board, timeLimit);
        }
    }

    void speed(Board::Type &board) {
		unsigned long int start = GetTickCount();
		incremental<White>(board, TM::depth(6));
		unsigned long int total = GetTickCount() - start;
		std::cout << "Search info - time: " << total << "ms (" << total / 1000.0 << "s), nodes: " << totalNodes << ", NPS: " << totalNodes / total << "K." << "\n";
		std::cout << "Score: " << totalNodes / total << "\n";
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
            if (!PeekNamedPipe(stdin_h, NULL, 0, NULL, &val, NULL)) {
                return true;
            }
            return val > 0;
        } else {
            GetNumberOfConsoleInputEvents(stdin_h, &val);
            return val > 1;
        }

        return false;
    #endif
    }  

    bool stopInterupter()
    {
        if(totalNodes % 1024 == 0 && isInputAvailable())
        {
            std::string input;
            std::getline(std::cin, input);
            return (input == "stop");
        }

        return false;
    }

    bool timeInterupter()
    {
		return GetTickCount() >= endTime || stopInterupter();
    }
}

#endif /* SEARCH_HPP */
