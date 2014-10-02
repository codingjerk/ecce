#ifndef EVAL_HPP
#define EVAL_HPP

#include "score.hpp"
#include "boards.hpp"
#include "colors.hpp"
#include "PST.hpp"

namespace Eval {
    template <Color::Type WHO> 
    Score::Type material(const Board::Type& board) {
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
    Score::Type positional(const Board::Type &board) {
        Score::Type result = Score::Draw;

        forCoord(x)
        forCoord(y) {
            const auto square = Coord::create(x, y);
            const auto piece = board.squares[square];
			if (piece == Piece::null) continue;

            const auto PSTValue = PST::tables[piece][square];

            result += PSTValue;
        }

        if (WHO == White) {
            return result;
        } else {
            return -result;
        }
    }
	
    template <Color::Type WHO>
	Score::Type total(const Board::Type &board) {
		return material(board) + positional(board);
	}
}

#endif /* EVAL_HPP */
