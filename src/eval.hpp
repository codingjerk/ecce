#ifndef EVAL_HPP
#define EVAL_HPP

#include "score.hpp"
#include "boards.hpp"
#include "colors.hpp"

namespace Eval {
	template <Color::Type C> Score::Type material(const Board::Type &board);

	template <> Score::Type material<White>(const Board::Type& board) {
		Score::Type result = 0;

		result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Pawn)])   * Score::Pawn;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Knight)]) * Score::Knight;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Bishop)]) * Score::Bishop;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Rook)])   * Score::Rook;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Queen)])  * Score::Queen;
    
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Pawn)])   * Score::Pawn;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Knight)]) * Score::Knight;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Bishop)]) * Score::Bishop;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Rook)])   * Score::Rook;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Queen)])  * Score::Queen;

		return result;
	}

	template <> Score::Type material<Black>(const Board::Type& board) {
		Score::Type result = 0;
    
		result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Pawn)])   * Score::Pawn;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Knight)]) * Score::Knight;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Bishop)]) * Score::Bishop;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Rook)])   * Score::Rook;
		result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Queen)])  * Score::Queen;

		result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Pawn)])   * Score::Pawn;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Knight)]) * Score::Knight;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Bishop)]) * Score::Bishop;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Rook)])   * Score::Rook;
		result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Queen)])  * Score::Queen;

		return result;
	}
}

#endif /* EVAL_HPP */
