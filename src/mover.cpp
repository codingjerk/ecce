#include "mover.hpp"

#include "generatorTables.hpp"
#include "checker.hpp"
#include "castles.hpp"

#include <iostream>

//                         [from coord] [to coord]
Castle::Type castleChanging[makeUNumspeed(1) << Coord::usedBits][makeUNumspeed(1) << Coord::usedBits];

Boolspeed makeUsual(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Board::enpassant(board, Enpassant::null);

    if (Move::isCapture(move)) Board::removePiece(board, to);
    
    Board::setPiece(board, board.squares[from], to);

    Board::removePiece(board, from);

    return makeBoolspeed(1);
}

Boolspeed makePromotion(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Board::enpassant(board, Enpassant::null);

    if (Move::isCapture(move)) Board::removePiece(board, to);
    
    const auto promoted = (move & Move::promotionMask) >> Move::promotionOffset;
    Board::setPiece(board, promoted, to);

    Board::removePiece(board, from);

    return makeBoolspeed(1);
}

Boolspeed makePawnDoubleWhite(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle);

    Board::enpassant(board, to - 8ull);

    Board::setPiece<White|Pawn>(board, to);

    Board::removePiece<White|Pawn>(board, from);

    return makeBoolspeed(1);
}

Boolspeed makePawnDoubleBlack(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle);

    Board::enpassant(board, to + 8ull);

    Board::setPiece<Black|Pawn>(board, to);

    Board::removePiece<Black|Pawn>(board, from);

    return makeBoolspeed(1);
}

Boolspeed makeEnpassantWhite(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle);

    Board::enpassant(board, Enpassant::null);

    Board::removePiece<Black|Pawn>(board, to - 8ull);
    Board::setPiece<White|Pawn>(board, to);
    Board::removePiece<White|Pawn>(board, from);

    return makeBoolspeed(1);
}

Boolspeed makeEnpassantBlack(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle);

    Board::enpassant(board, Enpassant::null);

    Board::removePiece<White|Pawn>(board, to + 8ull);
    Board::setPiece<Black|Pawn>(board, to);
    Board::removePiece<Black|Pawn>(board, from);

    return makeBoolspeed(1);
}

Boolspeed makeCastleWhiteLong(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle & ~(Castle::whiteKing | Castle::whiteQueen));

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<White|King>(board, Coord::C1);
    Board::removePiece<White|King>(board, Coord::E1);

    Board::setPiece<White|Rook>(board, Coord::D1);
    Board::removePiece<White|Rook>(board, Coord::A1);

    if (Checker::isAttacked<White>(board, Coord::D1)) return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, Coord::E1)) return makeBoolspeed(0);

    return makeBoolspeed(1);
}

Boolspeed makeCastleWhiteShort(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle & ~Castle::white);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<White|King>(board, Coord::G1);
    Board::removePiece<White|King>(board, Coord::E1);

    Board::setPiece<White|Rook>(board, Coord::F1);
    Board::removePiece<White|Rook>(board, Coord::H1);

    if (Checker::isAttacked<White>(board, Coord::F1)) return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, Coord::E1)) return makeBoolspeed(0);

    return makeBoolspeed(1);
}

Boolspeed makeCastleBlackLong(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle & ~Castle::black);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<Black|King>(board, Coord::C8);
    Board::removePiece<Black|King>(board, Coord::E8);

    Board::setPiece<Black|Rook>(board, Coord::D8);
    Board::removePiece<Black|Rook>(board, Coord::A8);

    if (Checker::isAttacked<Black>(board, Coord::D8)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, Coord::E8)) return makeBoolspeed(0);

    return makeBoolspeed(1);
}

Boolspeed makeCastleBlackShort(Move::Type, Board::Type& board) {
    const auto oldCastle = Board::castle(board);

    ++board.depthPtr;

    Board::castle(board, oldCastle & ~Castle::black);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece<Black|King>(board, Coord::G8);
    Board::removePiece<Black|King>(board, Coord::E8);

    Board::setPiece<Black|Rook>(board, Coord::F8);
    Board::removePiece<Black|Rook>(board, Coord::H8);

    if (Checker::isAttacked<Black>(board, Coord::F8)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, Coord::E8)) return makeBoolspeed(0);

    return makeBoolspeed(1);
}

Boolspeed (*Move::specialMakeWhite[6])(Move::Type, Board::Type&) = {
    makeUsual,
    makeEnpassantWhite,
    makeCastleWhiteLong,
    makeCastleWhiteShort,
    makePawnDoubleWhite,
    makePromotion
};

Boolspeed (*Move::specialMakeBlack[6])(Move::Type, Board::Type&) = {
    makeUsual,
    makeEnpassantBlack,
    makeCastleBlackLong,
    makeCastleBlackShort,
    makePawnDoubleBlack,
    makePromotion
};

void unmakeUsual(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);
    
    if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakeCastleWhiteShort(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<White|King>(board, Coord::E1);
    Board::removePiece<White|King>(board, Coord::G1);

    Board::setPiece<White|Rook>(board, Coord::H1);
    Board::removePiece<White|Rook>(board, Coord::F1);
}

void unmakeCastleWhiteLong(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<White|King>(board, Coord::E1);
    Board::removePiece<White|King>(board, Coord::C1);

    Board::setPiece<White|Rook>(board, Coord::A1);
    Board::removePiece<White|Rook>(board, Coord::D1);
}

void unmakeCastleBlackShort(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<Black|King>(board, Coord::E8);
    Board::removePiece<Black|King>(board, Coord::G8);

    Board::setPiece<Black|Rook>(board, Coord::H8);
    Board::removePiece<Black|Rook>(board, Coord::F8);
}

void unmakeCastleBlackLong(Move::Type, Board::Type& board) {
    --board.depthPtr;

    Board::setPiece<Black|King>(board, Coord::E8);
    Board::removePiece<Black|King>(board, Coord::C8);

    Board::setPiece<Black|Rook>(board, Coord::A8);
    Board::removePiece<Black|Rook>(board, Coord::D8);
}

void unmakePawnDouble(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);
}

void unmakePromotionWhite(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<White|Pawn>(board, from);

    Board::removePiece(board, to);

    if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakePromotionBlack(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<Black|Pawn>(board, from);

    Board::removePiece(board, to);

    if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakeEnpassantWhite(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<White|Pawn>(board, from);
    Board::removePiece<White|Pawn>(board, to);
    Board::setPiece<Black|Pawn>(board, to - 8ull);
}

void unmakeEnpassantBlack(Move::Type move, Board::Type& board) {
    --board.depthPtr;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece<Black|Pawn>(board, from);
    Board::removePiece<Black|Pawn>(board, to);
    Board::setPiece<White|Pawn>(board, to + 8ull);
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
