#ifndef EVAL_HPP
#define EVAL_HPP

#include "score.hpp"
#include "boards.hpp"
#include "bitboards.hpp"
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

	#define MAKE_PST_CALC(main, color, piece) {\
		Bitboard::Type wb = board.bitboards[Piece::create(color, piece)];\
		while (wb) {\
			Coord::Type coord = Bitboard::bitScan(wb);\
			wb ^= Bitboard::fromCoord(coord);\
			if (main == White) {\
				result += PST::tables[Piece::create(color, piece)][coord];\
			} else {\
				result -= PST::tables[Piece::create(color, piece)][coord];\
			}\
		}\
	}
    
    template <Color::Type WHO> 
    Score::Type positional(const Board::Type &board) {
		MAKEOPP(WHO);
        Score::Type result = Score::Draw;
		
		MAKE_PST_CALC(WHO, WHO, Pawn);
		MAKE_PST_CALC(WHO, WHO, Bishop);
		MAKE_PST_CALC(WHO, WHO, Knight);
		MAKE_PST_CALC(WHO, WHO, Rook);
		MAKE_PST_CALC(WHO, WHO, Queen);
		MAKE_PST_CALC(WHO, WHO, King);
		
		MAKE_PST_CALC(WHO, OPP, Pawn);
		MAKE_PST_CALC(WHO, OPP, Bishop);
		MAKE_PST_CALC(WHO, OPP, Knight);
		MAKE_PST_CALC(WHO, OPP, Rook);
		MAKE_PST_CALC(WHO, OPP, Queen);
		MAKE_PST_CALC(WHO, OPP, King);

		return result;
    }
    
    template <Color::Type WHO>
    Score::Type total(const Board::Type &board) {
        return material<WHO>(board) + positional<WHO>(board);
    }
}

#endif /* EVAL_HPP */
