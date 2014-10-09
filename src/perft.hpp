#ifndef PERFT_HPP
#define PERFT_HPP

#include "checker.hpp"
#include "generator.hpp"

namespace Perft {
    template <Color::Type COLOR>
    UNum64 perft_quiet(Board::Type &board, UNumspeed depth) {
        MAKEOPP(COLOR);
        if (Checker::isCheck<OPP>(board)) return 0;

        if (depth == 0) return 1;
        UNum64 result = 0;

		Generator::forBoard<COLOR>(Board::currentBuffer(board), board);
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
		
		Generator::forBoard<COLOR>(Board::currentBuffer(board), board);
        UNumspeed total = Board::currentBuffer(board)[0];
        for (UNumspeed i = 1; i <= total; ++i) {
			const Move::Type move = Board::currentBuffer(board)[i];
            if (Move::make<COLOR>(move, board)) {
                const auto nodes = perft_quiet<OPP>(board, depth - 1);
                std::cout << "Move: " << Move::show(move) << " = " << nodes << "\n";
                result += nodes;
            }
		
            Move::unmake<COLOR>(move, board);
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
