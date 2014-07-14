#ifndef PERFT_HPP
#define PERFT_HPP

namespace Perft {
    template <Color::Type COLOR>
    UNum64 perft_quiet(Generator::MoveBuffer *buffer, Board::Type &board, UNumspeed depth) {
        const Color::Type OPP = COLOR == White? Black: White;
        if (Checker::isCheck<OPP>(board)) return 0;

        if (depth == 0) return 1;
        UNum64 result = 0;

        Generator::forBoard<COLOR>(buffer[depth], board);
        UNumspeed total = buffer[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>(buffer[depth][i], board))
                result += perft_quiet<OPP>(buffer, board, depth - 1);

            Move::unmake<COLOR>(buffer[depth][i], board);
        }

        return result;
    }

    UNum64 perft_quiet(Generator::MoveBuffer *buffer, Board::Type &board, UNumspeed depth) {
        if (board.turn == White) {
            return perft_quiet<White>(buffer, board, depth);
        } else {
            return perft_quiet<Black>(buffer, board, depth);
        }
    }

    template <Color::Type COLOR>
    UNum64 perft(Generator::MoveBuffer *buffer, Board::Type &board, UNumspeed depth) {
        const Color::Type OPP = COLOR == White? Black: White;

        if (depth == 0) return 1;
        UNum64 result = 0;

        Generator::forBoard<COLOR>(buffer[depth], board);
        UNumspeed total = buffer[depth][0];
        for (UNumspeed i = 1; i <= total; ++i) {
            if (Move::make<COLOR>(buffer[depth][i], board)) {
                const auto nodes = perft_quiet<OPP>(buffer, board, depth - 1);;
                std::cout << "Move: " << Move::show(buffer[depth][i]) << " = " << nodes << "\n";
                result += nodes;
            }

            Move::unmake<COLOR>(buffer[depth][i], board);
        }
        
        std::cout << "Perft at depth " << depth << " = " << result << "\n";

        return result;
    }

    UNum64 perft(Generator::MoveBuffer *buffer, Board::Type &board, UNumspeed depth) {
        if (board.turn == White) {
            return perft<White>(buffer, board, depth);
        } else {
            return perft<Black>(buffer, board, depth);
        }
    }
}

#endif /* PERFT_HPP */
