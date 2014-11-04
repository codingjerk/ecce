#ifndef PERFT_HPP
#define PERFT_HPP

#include "checker.hpp"
#include "generator.hpp"

#include <omp.h>

namespace Perft {
    template <Color::Type COLOR>
    UNum64 perft_quiet(Board::Type &board, UNumspeed depth) {
        MAKEOPP(COLOR);

        if (depth == 0) return 1;
        UNum64 result = 0;

        Generator::phase<COLOR>(Board::currentBuffer(board), board);
        UNumspeed total = Board::currentBuffer(board)[0];
        for (UNumspeed i = 1; i <= total; ++i) {
            const Move::Type move = Board::currentBuffer(board)[i];
            if (Move::make<COLOR>(move, board))
                result += perft_quiet<OPP>(board, depth - 1);

            Move::unmake<COLOR>(move, board);
        }

        return result;
    }

    inline UNum64 perft_quiet(Board::Type &board, UNumspeed depth) {
        if (board.turn == White) {
            return perft_quiet<White>(board, depth);
        } else {
            return perft_quiet<Black>(board, depth);
        }
    }

    template <Color::Type COLOR>
    UNum64 perft(Board::Type &board, UNumspeed depth) {
        MAKEOPP(COLOR);

        if (depth == 0) return 1;
        UNum64 result = 0;
        
        Generator::phase<COLOR>(Board::currentBuffer(board), board);
        Numspeed total = Board::currentBuffer(board)[0];

        #pragma omp parallel for reduction(+:result)
        for (Numspeed i = 1; i <= total; ++i) {
            const Move::Type move = Board::currentBuffer(board)[i];
            Board::Type newBoard;
            Board::copy(newBoard, board);
            if (Move::make<COLOR>(move, newBoard)) {
                const auto nodes = perft_quiet<OPP>(newBoard, depth - 1);
                std::cout << "Move: " << Move::show(move) << " = " << nodes << "\n";
                result += nodes;
            }
        }
        
        std::cout << "Perft at depth " << depth << " = " << result << "\n";

        return result;
    }

    inline UNum64 perft(Board::Type &board, UNumspeed depth) {
        if (board.turn == White) {
            return perft<White>(board, depth);
        } else {
            return perft<Black>(board, depth);
        }
    }
}

#endif /* PERFT_HPP */
