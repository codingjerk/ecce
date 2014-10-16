#include "mover.hpp"

#include "generatorTables.hpp"
#include "checker.hpp"
#include "castles.hpp"

#include <iostream>

//                         [from coord] [to coord]
Castle::Type castleChanging[makeUNumspeed(1) << Coord::usedBits][makeUNumspeed(1) << Coord::usedBits];

enum Triple {TFalse, TUnknown, TTrue};

template <Color::Type COLOR, Triple IS_CAPTURE>
Boolspeed makeUsual(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Board::enpassant(board, Enpassant::null);

    if (IS_CAPTURE == TUnknown) {
        if (Move::isCapture(move)) Board::removePiece<true>(board, to);
    } else if (IS_CAPTURE == TTrue) {
        Board::removePiece<true>(board, to);
    }
    
	Board::setPiece<true>(board, board.squares[from], to);

	Board::removePiece<true>(board, from);

    return !(Checker::isCheck<COLOR>(board));
}

template <Color::Type COLOR, Triple IS_CAPTURE>
Boolspeed makePromotion(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Board::enpassant(board, Enpassant::null);
    
    if (IS_CAPTURE == TUnknown) {
	    if (Move::isCapture(move)) Board::removePiece<true>(board, to);
    } else if (IS_CAPTURE == TTrue) {
        Board::removePiece<true>(board, to);
    }
    
    const auto promoted = (move & Move::promotionMask) >> Move::promotionOffset;
	Board::setPiece<true>(board, promoted, to);

	Board::removePiece<true>(board, from);
    
    return !(Checker::isCheck<COLOR>(board));
}

template <Color::Type COLOR>
Boolspeed makePawnDouble(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle);

    if (COLOR == White) {
        Board::enpassant(board, to - 8ull);
    } else {
        Board::enpassant(board, to + 8ull);
    }

    Board::setPiece<COLOR|Pawn, true>(board, to);

    Board::removePiece<COLOR|Pawn, true>(board, from);
    
    return !(Checker::isCheck<COLOR>(board));
}

Boolspeed makeEnpassantWhite(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle);

    Board::enpassant(board, Enpassant::null);

    Board::removePiece<Black|Pawn, true>(board, to - 8ull);
    Board::setPiece<White|Pawn, true>(board, to);
    Board::removePiece<White|Pawn, true>(board, from);
    
    return !(Checker::isCheck<White>(board));
}

Boolspeed makeEnpassantBlack(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle);

    Board::enpassant(board, Enpassant::null);

    Board::removePiece<White|Pawn, true>(board, to + 8ull);
    Board::setPiece<Black|Pawn, true>(board, to);
    Board::removePiece<Black|Pawn, true>(board, from);
    
    return !(Checker::isCheck<Black>(board));
}

Boolspeed makeCastleWhiteLong(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle & ~(Castle::whiteKing | Castle::whiteQueen));

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<White|King, true>(board, Coord::C1);
    Board::removePiece<White|King, true>(board, Coord::E1);

    Board::setPiece<White|Rook, true>(board, Coord::D1);
    Board::removePiece<White|Rook, true>(board, Coord::A1);

    if (Checker::isAttacked<White>(board, Coord::D1)) return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, Coord::E1)) return makeBoolspeed(0);
    
    return !(Checker::isCheck<White>(board));
}

Boolspeed makeCastleWhiteShort(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle & ~Castle::white);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<White|King, true>(board, Coord::G1);
    Board::removePiece<White|King, true>(board, Coord::E1);

    Board::setPiece<White|Rook, true>(board, Coord::F1);
    Board::removePiece<White|Rook, true>(board, Coord::H1);

    if (Checker::isAttacked<White>(board, Coord::F1)) return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, Coord::E1)) return makeBoolspeed(0);
    
    return !(Checker::isCheck<White>(board));
}

Boolspeed makeCastleBlackLong(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle & ~Castle::black);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<Black|King, true>(board, Coord::C8);
    Board::removePiece<Black|King, true>(board, Coord::E8);

    Board::setPiece<Black|Rook, true>(board, Coord::D8);
    Board::removePiece<Black|Rook, true>(board, Coord::A8);

    if (Checker::isAttacked<Black>(board, Coord::D8)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, Coord::E8)) return makeBoolspeed(0);
    
    return !(Checker::isCheck<Black>(board));
}

Boolspeed makeCastleBlackShort(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

	Board::copyzobrist(board);
    ++board.depthPtr;

    Board::castle(board, oldCastle & ~Castle::black);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<Black|King, true>(board, Coord::G8);
    Board::removePiece<Black|King, true>(board, Coord::E8);

    Board::setPiece<Black|Rook, true>(board, Coord::F8);
    Board::removePiece<Black|Rook, true>(board, Coord::H8);

    if (Checker::isAttacked<Black>(board, Coord::F8)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, Coord::E8)) return makeBoolspeed(0);

    return !(Checker::isCheck<Black>(board));
}

Boolspeed (*Move::specialMakeWhite[6])(Move::Type, Board::Type&) = {
    makeUsual<White, TUnknown>,
    makeEnpassantWhite,
    makeCastleWhiteLong,
    makeCastleWhiteShort,
    makePawnDouble<White>,
    makePromotion<White, TUnknown>
};

Boolspeed (*Move::specialMakeBlack[6])(Move::Type, Board::Type&) = {
    makeUsual<Black, TUnknown>,
    makeEnpassantBlack,
    makeCastleBlackLong,
    makeCastleBlackShort,
    makePawnDouble<Black>,
    makePromotion<Black, TUnknown>
};

Boolspeed (*Move::specialMakeCaptureWhite[6])(Move::Type, Board::Type&) = {
    makeUsual<White, TTrue>,
    makeEnpassantWhite,
    nullptr,
    nullptr,
    nullptr,
    makePromotion<White, TTrue>
};

Boolspeed (*Move::specialMakeCaptureBlack[6])(Move::Type, Board::Type&) = {
    makeUsual<Black, TTrue>,
    makeEnpassantBlack,
    nullptr,
    nullptr,
    nullptr,
    makePromotion<Black, TTrue>
};

void unmakeUsual(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<false>(board, board.squares[to], from);
	Board::removePiece<false>(board, to);
    
	if (Move::isCapture(move)) Board::setPiece<false>(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakeUsualCapture(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

	Board::setPiece<false>(board, board.squares[to], from);
	Board::removePiece<false>(board, to);
    
	Board::setPiece<false>(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakeCastleWhiteShort(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<White|King, false>(board, Coord::E1);
    Board::removePiece<White|King, false>(board, Coord::G1);

    Board::setPiece<White|Rook, false>(board, Coord::H1);
    Board::removePiece<White|Rook, false>(board, Coord::F1);
}

void unmakeCastleWhiteLong(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<White|King, false>(board, Coord::E1);
    Board::removePiece<White|King, false>(board, Coord::C1);

    Board::setPiece<White|Rook, false>(board, Coord::A1);
    Board::removePiece<White|Rook, false>(board, Coord::D1);
}

void unmakeCastleBlackShort(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<Black|King, false>(board, Coord::E8);
    Board::removePiece<Black|King, false>(board, Coord::G8);

    Board::setPiece<Black|Rook, false>(board, Coord::H8);
    Board::removePiece<Black|Rook, false>(board, Coord::F8);
}

void unmakeCastleBlackLong(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<Black|King, false>(board, Coord::E8);
    Board::removePiece<Black|King, false>(board, Coord::C8);

    Board::setPiece<Black|Rook, false>(board, Coord::A8);
    Board::removePiece<Black|Rook, false>(board, Coord::D8);
}

void unmakePawnDouble(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<false>(board, board.squares[to], from);
	Board::removePiece<false>(board, to);
}

void unmakePromotionWhite(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<White|Pawn, false>(board, from);

	Board::removePiece<false>(board, to);

	if (Move::isCapture(move)) Board::setPiece<false>(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakePromotionBlack(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<Black|Pawn, false>(board, from);

	Board::removePiece<false>(board, to);

	if (Move::isCapture(move)) Board::setPiece<false>(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakePromotionCaptureWhite(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<White|Pawn, false>(board, from);

	Board::removePiece<false>(board, to);

	Board::setPiece<false>(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakePromotionCaptureBlack(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<Black|Pawn, false>(board, from);

	Board::removePiece<false>(board, to);

	Board::setPiece<false>(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakeEnpassantWhite(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<White|Pawn, false>(board, from);
    Board::removePiece<White|Pawn, false>(board, to);
    Board::setPiece<Black|Pawn, false>(board, to - 8ull);
}

void unmakeEnpassantBlack(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<Black|Pawn, false>(board, from);
    Board::removePiece<Black|Pawn, false>(board, to);
    Board::setPiece<White|Pawn, false>(board, to + 8ull);
}

void (*Move::specialUnmakeWhite[6])(Move::Type, Board::Type&) = {
    unmakeUsual,
    unmakeEnpassantWhite,
    unmakeCastleWhiteLong,
    unmakeCastleWhiteShort,
    unmakePawnDouble,
    unmakePromotionWhite
};

void (*Move::specialUnmakeBlack[6])(Move::Type, Board::Type&) = {
    unmakeUsual,
    unmakeEnpassantBlack,
    unmakeCastleBlackLong,
    unmakeCastleBlackShort,
    unmakePawnDouble,
    unmakePromotionBlack
};

void (*Move::specialUnmakeCaptureWhite[6])(Move::Type, Board::Type&) = {
    unmakeUsualCapture,
    unmakeEnpassantWhite,
    nullptr,
    nullptr,
    nullptr,
    unmakePromotionCaptureWhite
};

void (*Move::specialUnmakeCaptureBlack[6])(Move::Type, Board::Type&) = {
    unmakeUsualCapture,
    unmakeEnpassantBlack,
    nullptr,
    nullptr,
    nullptr,
    unmakePromotionCaptureBlack
};

void Move::initTables() {
    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        
        forCoord(x)
        forCoord(y) {
            const auto to = Coord::create(x, y);

            auto castle = Castle::all;

            if (from == Coord::E8 || to == Coord::E8) {
                castle &= ~(Castle::black);
            } 

            if (from == Coord::E1 || to == Coord::E1) {
                castle &= ~(Castle::white);
            } 

            if (from == Coord::A8 || to == Coord::A8) {
                castle &= ~(Castle::blackQueen);
            }

            if (from == Coord::A1 || to == Coord::A1) {
                castle &= ~(Castle::whiteQueen);
            }

            if (from == Coord::H8 || to == Coord::H8) {
                castle &= ~(Castle::blackKing);
            }

            if (from == Coord::H1 || to == Coord::H1) {
                castle &= ~(Castle::whiteKing);
            }

            castleChanging[from][to] = castle;
        }
    }
}
