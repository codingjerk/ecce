#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "tm.hpp"
#include "moves.hpp"
#include "checker.hpp"
#include "score.hpp"
#include "eval.hpp"
#include "pv.hpp"
#include "interupters.hpp"
#include "quies.hpp"

namespace Search {
    template <Color::Type COLOR, Interupter isInterupt>
    Score::Type alphaBeta(Board::Type &board, Score::Type alpha, Score::Type beta, UNumspeed depth, Numspeed pvIndex) {
        ++totalNodes;
        MAKEOPP(COLOR);

        if (depth == 0) return quiesce<COLOR>(board, alpha, beta);
        if (Checker::isCheck<OPP>(board)) return Score::makeMate(Board::ply(board));

        if (isInterupt() || stopSearch) {
            stopSearch = true;
            return 0;
        }

        PV::master[pvIndex] = 0;
        Generator::phase<COLOR>(Board::currentBuffer(board), board);
        UNumspeed total = Board::currentBuffer(board)[0];
        Move::Type move;
        Score::Type score;
        for (UNumspeed i = 1; i <= total; ++i) {
            move = Board::currentBuffer(board)[i];

            if (Move::make<COLOR>(move, board)) {
                score = -alphaBeta<OPP, isInterupt>(board, -beta, -alpha, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));

                if (score > alpha) {
                    alpha = score;
                    PV::master[pvIndex] = move;
                    PV::copy(PV::master + pvIndex + 1, PV::master + pvIndex + MAX_DEPTH - Board::ply(board), MAX_DEPTH - Board::ply(board) - 1);
                }
            }

            Move::unmake<COLOR>(move, board);

            if (alpha >= beta) break;
        }

        return alpha;
    }

    template <Color::Type COLOR>
    Move::Type simple(Board::Type &board, TM::DepthLimit depth) {
        stopSearch = false;

        auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth.maxDepth, 0);

        std::cout << "info depth " << depth.maxDepth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush; 

        return PV::master[0];
    }

    inline Move::Type simple(Board::Type &board, TM::DepthLimit depth) {
        if (board.turn == White) {
            return simple<White>(board, depth);
        } else {
            return simple<Black>(board, depth);
        }
    }

    template <Color::Type COLOR>
    Move::Type incremental(Board::Type &board, TM::DepthLimit depthLimit) {
        PV::clear();

        stopSearch = false;

        Move::Type bestMove = Move::create(Coord::A1, Coord::A1, Piece::null);

        for (Numspeed depth = 1; depth <= depthLimit.maxDepth; ++depth) {
            totalNodes = 0;
            auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
        
            if (stopSearch) break;
            
            std::cout << "info depth " << depth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush;

            bestMove = PV::master[0];
        }

        return bestMove;
    }

    inline Move::Type incremental(Board::Type &board, TM::DepthLimit depth) {
        if (board.turn == White) {
            return incremental<White>(board, depth);
        } else {
            return incremental<Black>(board, depth);
        }
    }

    template <Color::Type COLOR>
    Move::Type incremental(Board::Type &board, TM::TimeLimit timeLimit) {
        PV::clear();

        stopSearch = false;

        Move::Type bestMove = Move::create(Coord::A1, Coord::A1, Piece::null);

        endTime = GetTickCount() + timeLimit.maxTime;
        for (UNumspeed depth = 1; depth <= MAX_DEPTH; ++depth) {
            totalNodes = 0;
            auto score = alphaBeta<COLOR, timeInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
        
            if (stopSearch) break;

            std::cout << "info depth " << depth << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush;

            bestMove = PV::master[0];
        }

        return bestMove;
    }

    inline Move::Type incremental(Board::Type &board, TM::TimeLimit timeLimit) {
        if (board.turn == White) {
            return incremental<White>(board, timeLimit);
        } else {
            return incremental<Black>(board, timeLimit);
        }
    }

    inline void speed(Board::Type &board) {
        unsigned long int start = GetTickCount();
        incremental<White>(board, TM::depth(7));
        unsigned long int total = GetTickCount() - start;
        std::cout << "Search info - time: " << total << "ms (" << total / 1000.0 << "s), nodes: " << totalNodes << ", NPS: " << totalNodes / total << "K." << "\n";
        std::cout << "Score: " << totalNodes / total << "\n";
    }
}

#endif /* SEARCH_HPP */
