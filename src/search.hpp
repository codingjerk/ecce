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
    #define min(a,b) ((a)<(b))?(a):(b)
#else
    #include <Windows.h>
#endif

namespace Search {
	typedef bool (*Interupter)();  

    bool stopSearch;
	UNumspeed totalNodes;
	UNumspeed endTime;

    bool isInputAvailable();

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
	
	const UNumspeed PV_SIZE = (MAX_DEPTH * MAX_DEPTH + MAX_DEPTH) / 2;
	Move::Type pvArray[PV_SIZE];

	void movcpy(Move::Type *pTarget, const Move::Type *pSource, int n) {
		while (n-- && (*pTarget++ = *pSource++));
	}

    template <Color::Type COLOR, Interupter isInterupt>
    Score::Type alphaBeta(Board::Type &board, Score::Type alpha, Score::Type beta, Numspeed depth, Numspeed pvIndex) {
		++totalNodes;
        MAKEOPP(COLOR);
        if (Checker::isCheck<OPP>(board)) return Score::Infinity - MAX_DEPTH + Board::ply(board);

        if (depth <= 0) return Eval::total<COLOR>(board);

		if (isInterupt() || stopSearch) {
			stopSearch = true;
			return 0;
		}

		pvArray[pvIndex] = 0;
		Generator::forBoard<COLOR>(Board::currentBuffer(board), board);
        UNumspeed total = Board::currentBuffer(board)[0];
		Move::Type move;
		Score::Type score;
        for (UNumspeed i = 1; i <= total; ++i) {
			move = Board::currentBuffer(board)[i];

            if (Move::make<COLOR>(move, board)) {
                score = -alphaBeta<OPP, isInterupt>(board, -beta, -alpha, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));

                if (score > alpha) {
                    alpha = score;
					pvArray[pvIndex] = move;
					movcpy(pvArray + pvIndex + 1, pvArray + pvIndex + MAX_DEPTH - Board::ply(board), MAX_DEPTH - Board::ply(board) - 1);
                }
            }

            Move::unmake<COLOR>(move, board);

            if (alpha >= beta) break;
        }

        return alpha;
    }

    std::string showPV() {
        std::string result;

		Move::Type move;
		for (int i = 0; move = pvArray[i]; ++i) {
            result += Move::show(move) + " ";
        }

        return result;
    }

    template <Color::Type COLOR>
    Move::Type simple(Board::Type &board, TM::DepthLimit depth) {
        stopSearch = false;

		auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth.maxDepth, 0);

        std::cout << "info depth " << depth.maxDepth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << showPV() << "\n" << std::flush; 

        return pvArray[0];
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
		for (int i = 0; i < PV_SIZE; ++i) {
			pvArray[i] = 0;
		}

        stopSearch = false;

        Move::Type bestMove = Move::create(Coord::A1, Coord::A1, Piece::null);

        for (Numspeed depth = 1; depth <= depthLimit.maxDepth; ++depth) {
			totalNodes = 0;
            auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
        
            if (stopSearch) break;
			
			std::cout << "info depth " << depth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << showPV() << "\n" << std::flush;

            bestMove = pvArray[0];
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
		for (int i = 0; i < PV_SIZE; ++i) {
			pvArray[i] = 0;
		}

        stopSearch = false;

        Move::Type bestMove = Move::create(Coord::A1, Coord::A1, Piece::null);

		endTime = GetTickCount() + timeLimit.maxTime;
        for (Numspeed depth = 1; depth <= MAX_DEPTH; ++depth) {
			totalNodes = 0;
            auto score = alphaBeta<COLOR, timeInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
        
            if (stopSearch) break;

			std::cout << "info depth " << depth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << showPV() << "\n" << std::flush;

            bestMove = pvArray[0];
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
}

#endif /* SEARCH_HPP */
