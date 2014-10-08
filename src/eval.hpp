#ifndef EVAL_HPP
#define EVAL_HPP

#include "score.hpp"
#include "boards.hpp"
#include "bitboards.hpp"
#include "colors.hpp"
#include "PST.hpp"

namespace Eval {
    template <Color::Type WHO> 
    inline Score::Type material(const Board::Type& board) {
        const auto OPP = WHO == White? Black: White;
        Score::Type result = 0;

        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Pawn)])   * Score::Pawn;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Knight)]) * Score::Knight;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Bishop)]) * Score::Bishop;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Rook)])   * Score::Rook;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Queen)])  * Score::Queen;
    
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Pawn)])   * Score::Pawn;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Knight)]) * Score::Knight;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Bishop)]) * Score::Bishop;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Rook)])   * Score::Rook;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Queen)])  * Score::Queen;

        return result;
    }
    
    template <Color::Type WHO> 
    inline Score::Type positional(const Board::Type &board) {
		if (WHO == White) {
			return board.positionalScore;
		} else {
			return -board.positionalScore;
		}
    }
    
    template <Color::Type WHO>
    inline Score::Type total(const Board::Type &board) {
        return material<WHO>(board) + positional<WHO>(board);
    }
}

#endif /* EVAL_HPP */
