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
#include "generatorPhases.hpp"
#include "statistic.hpp"

namespace Search {
    #define NEGASCOUT

    template <Color::Type COLOR, Interupter isInterupt, bool ROOT = true>
    Score::Type alphaBeta(Board::Type &board, Score::Type alpha, Score::Type beta, UNumspeed depth, Numspeed pvIndex) {
        ++totalNodes;
        Statistic::increaseNodes();
        MAKEOPP(COLOR);
        
        if (!ROOT) {
            if (Board::isRepeat(board) || Board::isFifty(board)) {
                Statistic::repeatPruned();
                return Score::Draw;
            }
        }

        if (depth == 0) return quiesce<COLOR>(board, alpha, beta);
        
        if (!ROOT) {
            if (isInterupt() || stopSearch) {
                stopSearch = true;
                return 0;
            }
        }

        PV::master[pvIndex] = 0;
        Move::Type move;
        Score::Type score;
        bool noLegalMoves = true;
        forPhases(phase, Generator::phases<COLOR>()) {
            phase.generate(Board::currentBuffer(board), board);
            UNumspeed total = Board::currentBuffer(board)[0];
            for (UNumspeed i = 1; i <= total; ++i) {
                move = Board::currentBuffer(board)[i];

                if (phase.make(move, board)) {
                    #ifdef NEGASCOUT
                        if (noLegalMoves) {
                            score = -alphaBeta<OPP, isInterupt, false>(board, -beta, -alpha, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));
                            noLegalMoves = false;
                        } else {
                            score = -alphaBeta<OPP, isInterupt, false>(board, -alpha - 1, -alpha, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board)); 
                        
                            if (score > alpha && score < beta) {
                                Statistic::negaScoutFailed();
                                score = -alphaBeta<OPP, isInterupt, false>(board, -beta, -score, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));
                            }
                        }
                    #else
                        score = -alphaBeta<OPP, isInterupt, false>(board, -beta, -alpha, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));
                        noLegalMoves = false;
                    #endif

                    if (score > alpha) {
                        Statistic::alphaUpped();
                        alpha = score;
                        PV::master[pvIndex] = move;
                        PV::copy(PV::master + pvIndex + 1, PV::master + pvIndex + MAX_DEPTH - Board::ply(board), MAX_DEPTH - Board::ply(board) - 1);
                    } else {
                        Statistic::alphaPruned();
                    }
                }

                phase.unmake(move, board);

                if (alpha >= beta) {
                    Statistic::betaPruned();
                    return alpha;
                }
            }

            Statistic::goingToNextPhase();
        }

        if (noLegalMoves) {
            Statistic::noLegalMoves();
            if (Checker::isCheck<COLOR>(board)) {
                return -Score::makeMate(Board::ply(board));
            } else {
                return Score::Draw;
            }
        }

        Statistic::returnedAlpha();
        return alpha;
    }

    template <Color::Type COLOR>
    Move::Type simple(Board::Type &board, TM::DepthLimit depth) {
        stopSearch = false;
        
        totalNodes = 0;
        auto startTime = GetTickCount();
        auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth.maxDepth, 0);
        auto totalTime = GetTickCount() - startTime;

        auto totalNPS = (totalTime != 0)? (totalNodes * 1000 / totalTime): totalNodes;
        std::cout << "info depth " << depth.maxDepth << " time " << totalTime << " nps " << totalNPS << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush; 

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
        
        totalNodes = 0;
        auto startTime = GetTickCount();
        for (Numspeed depth = 1; depth <= depthLimit.maxDepth; ++depth) {
            auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
            auto totalTime = GetTickCount() - startTime;

            auto totalNPS = (totalTime != 0)? (totalNodes * 1000 / totalTime): totalNodes;
        
            if (stopSearch) break;
            
            std::cout << "info depth " << depth << " time " << totalTime << " nps " << totalNPS << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush;

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
        totalNodes = 0;
        auto startTime = GetTickCount();
        for (UNumspeed depth = 1; depth <= MAX_DEPTH; ++depth) {
            auto score = alphaBeta<COLOR, timeInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
            auto totalTime = GetTickCount() - startTime;

            auto totalNPS = (totalTime != 0)? (totalNodes * 1000 / totalTime): totalNodes;
        
            if (stopSearch) break;

            std::cout << "info depth " << depth << " time " << totalTime << " nps " << totalNPS << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush;

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
