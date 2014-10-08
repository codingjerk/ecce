#include "score.hpp"

Score::Type Score::pieceToScoreTable[makeUNumspeed(1) << Piece::usedBits];

void Score::initTables() {
	for (UNumspeed i = 0; i < sizeof(pieceToScoreTable); ++i) {
		pieceToScoreTable[i] = Score::Null;
	}

	pieceToScoreTable[Piece::create(White, Dignity::Pawn)]   = Score::Pawn;
	pieceToScoreTable[Piece::create(White, Dignity::Bishop)] = Score::Bishop;
	pieceToScoreTable[Piece::create(White, Dignity::Knight)] = Score::Knight;
	pieceToScoreTable[Piece::create(White, Dignity::Rook)]   = Score::Rook;
	pieceToScoreTable[Piece::create(White, Dignity::Queen)]  = Score::Queen;
	pieceToScoreTable[Piece::create(White, Dignity::King)]   = Score::Null;
		
	pieceToScoreTable[Piece::create(Black, Dignity::Pawn)]   = -Score::Pawn;
	pieceToScoreTable[Piece::create(Black, Dignity::Bishop)] = -Score::Bishop;
	pieceToScoreTable[Piece::create(Black, Dignity::Knight)] = -Score::Knight;
	pieceToScoreTable[Piece::create(Black, Dignity::Rook)]   = -Score::Rook;
	pieceToScoreTable[Piece::create(Black, Dignity::Queen)]  = -Score::Queen;
	pieceToScoreTable[Piece::create(Black, Dignity::King)]   = -Score::Null;
}
