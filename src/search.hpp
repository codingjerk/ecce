#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <algorithm>

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
#include "history.hpp"
#include "killers.hpp"
#include "hash.hpp"

namespace Search {
    template <Color::Type COLOR, Interupter isInterupt, bool ROOT = true, bool NULLMOVE_ALLOWED = true>
    Score::Type alphaBeta(Board::Type &board, Score::Type alpha, Score::Type beta, UNumspeed depth, Numspeed pvIndex) {
        ++totalNodes;
        Statistic::increaseNodes();
        MAKEOPP(COLOR);

		PV::master[pvIndex] = 0;
		if (!ROOT) {
            if (Board::isRepeat(board) || Board::isFifty(board)) {
                Statistic::repeatPruned();
                return Score::Draw;
            }

            if (Checker::isCheck<COLOR>(board)) {
                ++depth;
            }

            if (depth == 0) return quiesce<COLOR>(board, alpha, beta);

            if (isInterupt() || stopSearch) {
                stopSearch = true;
                return 0;
            }

            const auto &hashNode = Hash::read(board.depthPtr->zobrist);
            if (hashNode.depth >= depth) {
                Statistic::hashFinded();
                PV::master[pvIndex] = hashNode.bestMove;
                auto score = hashNode.score;
                if (Score::isPositiveMate(score))      score -= Board::ply(board);
                else if (Score::isNegativeMate(score)) score += Board::ply(board);

                if (hashNode.type == Hash::Node::Beta || hashNode.type == Hash::Node::Exact) {
                    alpha = max(alpha, score);
                    if (alpha >= beta) {
                        Statistic::hashBetaPruned();
                        return beta;
                    }
                }
                else if (hashNode.type == Hash::Node::Alpha) {
                    if (score <= alpha) {
                        Statistic::hashAlphaPruned();
                        return alpha;
                    }
                }
            }

            if (NULLMOVE_ALLOWED && Board::ply(board) >= 4 && !Checker::isCheck<COLOR>(board)) {
                auto staticScore = Eval::total<COLOR>(board);

                if (staticScore >= beta) {
                    Statistic::nullMoveUsed();
                    Move::makeNull<COLOR>(board);
                    auto newDepth = (depth <= 4) ? 0 : depth - 4;
                    auto nullScore = -alphaBeta<OPP, isInterupt, false, false>(board, -beta, -beta + 1, newDepth, pvIndex + MAX_DEPTH - Board::ply(board));
                    Move::unmakeNull(board);

                    if (nullScore >= beta) {
                        return beta;
                    }
                    else {
                        Statistic::nullMoveFailed();
                    }
                }
            }
        }

        Move::Type move;
        Score::Type score;
        bool noLegalMoves = true;
        forPhases(phase, Generator::phases<COLOR>()) {
            phase.generate(Board::buffer(board), board);
            UNumspeed total = Board::buffer(board)[0];
            for (UNumspeed i = 1; i <= total; ++i) {
                move = Board::buffer(board)[i];

                if (phase.make(move, board)) {
                    if (noLegalMoves) {
                        score = -alphaBeta<OPP, isInterupt, false, true>(board, -beta, -alpha, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));
                        noLegalMoves = false;
                    } else {
                        score = -alphaBeta<OPP, isInterupt, false, true>(board, -alpha - 1, -alpha, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));
                        
                        if (score > alpha && score < beta) {
                            Statistic::negaScoutFailed();
                            score = -alphaBeta<OPP, isInterupt, false, true>(board, -beta, -score, depth - 1, pvIndex + MAX_DEPTH - Board::ply(board));
                        }
                    }

                    if (score > alpha) {
                        if (!Move::isCapture(move)) {
                            History::alphed(move, depth);
                            Killer::write(move, board);
                        }

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
                    if (!Move::isCapture(move)) History::beted(move, depth);
                    if (!stopSearch) Hash::write(board.depthPtr->zobrist, move, alpha, depth, Hash::Node::Beta, Board::ply(board));

                    Statistic::betaPruned();
                    return alpha;
                }
            }

            Statistic::goingToNextPhase();
        }

        if (noLegalMoves) {
            Statistic::noLegalMoves();
            Score::Type score;

            if (Checker::isCheck<COLOR>(board)) {
                score = -Score::makeMate(Board::ply(board));
            } else {
                score = Score::Draw;
            }
            
            if (!stopSearch) Hash::write(board.depthPtr->zobrist, 0, score, depth, Hash::Node::Exact, Board::ply(board));

            return score;
        }

        if (!stopSearch) {
            if (PV::master[pvIndex] != 0) {
                Hash::write(board.depthPtr->zobrist, PV::master[pvIndex], alpha, depth, Hash::Node::Exact, Board::ply(board));
            } else {
                Hash::write(board.depthPtr->zobrist, 0, alpha, depth, Hash::Node::Alpha, Board::ply(board));
            }
        }

        Statistic::returnedAlpha();
        return alpha;
    }

    #define INIT() \
        stopSearch = false; \
        Move::Type bestMove = 0; \
        totalNodes = 0; \
        auto startTime = GetTickCount();

    template <Color::Type COLOR>
    Move::Type incremental(Board::Type &board, TM::DepthLimit depthLimit) {
        INIT();

        for (Numspeed depth = 1; depth <= depthLimit.maxDepth; ++depth) {
            auto score = alphaBeta<COLOR, stopInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
            
            auto totalTime = GetTickCount() - startTime;
            auto totalNPS = (totalTime != 0)? (totalNodes * 1000 / totalTime): totalNodes;
        
            if (stopSearch) {
                if (depth == 1) bestMove = PV::master[0];
                break;
            }
            
            if (totalTime >= 100) std::cout << "info depth " << depth << " time " << totalTime << " hashfull " << Hash::fillFactor() << " nps " << totalNPS << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush;

            bestMove = PV::master[0];
        }

        return bestMove;
    }

    inline Move::Type incremental(Board::Type &board, const TM::DepthLimit &depth) {
        if (board.turn == White) {
            return incremental<White>(board, depth);
        } else {
            return incremental<Black>(board, depth);
        }
    }

    template <Color::Type COLOR>
    Move::Type incremental(Board::Type &board, TM::TimeLimit timeLimit) {
        INIT();
        endTime = GetTickCount() + timeLimit.real;

        for (UNumspeed depth = 1; depth <= MAX_DEPTH; ++depth) {
            auto score = alphaBeta<COLOR, timeInterupter>(board, -Score::Infinity, Score::Infinity, depth, 0);
            
            auto totalTime = GetTickCount() - startTime;
            auto totalNPS = (totalTime != 0)? (totalNodes * 1000 / totalTime): totalNodes;

            if (stopSearch) {
                if (depth == 1) bestMove = PV::master[0];
                break;
            }

            if (totalTime >= 100) std::cout << "info depth " << depth << " time " << totalTime << " hashfull " << Hash::fillFactor() << " nps " << totalNPS << " nodes " << totalNodes << " score " << Score::show(score) << " pv " << PV::show() << "\n" << std::flush;

            bestMove = PV::master[0];
        }

        return bestMove;
    }

    inline Move::Type incremental(Board::Type &board, const TM::TimeLimit &timeLimit) {
        if (board.turn == White) {
            return incremental<White>(board, timeLimit);
        } else {
            return incremental<Black>(board, timeLimit);
        }
    }

    inline void clear() {
        PV::clear();
        Hash::clear();
        History::split();
    }

    inline void flush() {
        PV::clear();
        Hash::clear();
        History::flush();
    }

    inline void speed() {
        Board::Type board;
        Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        unsigned long int start = GetTickCount();
        incremental<White>(board, TM::depth(11));
        unsigned long int total = GetTickCount() - start;
        auto score = totalNodes / total;
        std::cout << "\nSearch info - time: " << total << "ms (" << total / 1000.0 << "s), nodes: " << totalNodes << ", NPS: " << totalNodes / total << "K." << "\n\n";
        flush();

        Board::setFromFen(board, "8/4p3/p2p4/2pP4/2P1P3/1P4k1/1P1K4/8 w - - 0 1");

        start = GetTickCount();
        incremental<White>(board, TM::depth(21));
        total = GetTickCount() - start;
        score += totalNodes / total;
        std::cout << "\nSearch info - time: " << total << "ms (" << total / 1000.0 << "s), nodes: " << totalNodes << ", NPS: " << totalNodes / total << "K." << "\n\n";
        flush();

        Board::setFromFen(board, "1r1rb1k1/2p3pp/p2q1p2/3PpP1Q/Pp1bP2N/1B5R/1P4PP/2B4K w - - 0 1");

        start = GetTickCount();
        incremental<White>(board, TM::depth(11));
        total = GetTickCount() - start;
        score += totalNodes / total;
        std::cout << "\nSearch info - time: " << total << "ms (" << total / 1000.0 << "s), nodes: " << totalNodes << ", NPS: " << totalNodes / total << "K." << "\n\n";
        flush();

        Board::setFromFen(board, "8/7p/5k2/5p2/p1p2P2/Pr1pPK2/1P1R3P/8 b - - 0 1");

        start = GetTickCount();
        incremental<White>(board, TM::depth(15));
        total = GetTickCount() - start;
        score += totalNodes / total;
        std::cout << "\nSearch info - time: " << total << "ms (" << total / 1000.0 << "s), nodes: " << totalNodes << ", NPS: " << totalNodes / total << "K." << "\n\n";
        flush();

        std::cout << "\nScore: " << score << "\n";
    }
}

#endif /* SEARCH_HPP */
