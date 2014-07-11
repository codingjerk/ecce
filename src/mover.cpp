#include "mover.hpp"

#include "generatorTables.hpp"
#include "checker.hpp"

#include <iostream>

//@TODO(FAST): Make it template
Boolspeed Move::make(Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;
    // @TODO(FAST): rewrite this shit
    if ((board.squares[from] == Piece::create(White, Pawn))
     && (to == from + 16ull)) {// 2 lines up 
        Board::enpassant(board, to - 8ull);
    } else if (board.squares[from] == Piece::create(Black, Pawn)
            && to == from - 16ull) {// 2 lines up 
        Board::enpassant(board, to + 8ull);
    } else {
        Board::enpassant(board, Enpassant::null);
    }

    auto newCastle = oldCastle;
    if (board.squares[from] == Piece::create(Black, King)) {
        newCastle &= ~(Castle::blackQueen | Castle::blackKing);
    } else if (board.squares[from] == Piece::create(White, King)) {
        newCastle &= ~(Castle::whiteQueen | Castle::whiteKing);
    } else if (from == Coord::fromString("a8") && board.squares[from] == Piece::create(Black, Rook)) {
        newCastle &= ~(Castle::blackQueen);
    } else if (from == Coord::fromString("a1") && board.squares[from] == Piece::create(White, Rook)) {
        newCastle &= ~(Castle::whiteQueen);
    } else if (from == Coord::fromString("h8") && board.squares[from] == Piece::create(Black, Rook)) {
        newCastle &= ~(Castle::blackKing);
    } else if (from == Coord::fromString("h1") && board.squares[from] == Piece::create(White, Rook)) {
        newCastle &= ~(Castle::whiteKing);
    } else if (to == Coord::fromString("a8") && board.squares[to] == Piece::create(Black, Rook)) {
        newCastle &= ~(Castle::blackQueen);
    } else if (to == Coord::fromString("a1") && board.squares[to] == Piece::create(White, Rook)) {
        newCastle &= ~(Castle::whiteQueen);
    } else if (to == Coord::fromString("h8") && board.squares[to] == Piece::create(Black, Rook)) {
        newCastle &= ~(Castle::blackKing);
    } else if (to == Coord::fromString("h1") && board.squares[to] == Piece::create(White, Rook)) {
        newCastle &= ~(Castle::whiteKing);
    }
    Board::castle(board, newCastle);

    //@TODO(FAST): Rewrite
    if (Move::isEnpassant(move)) {
        Piece::Type captured = (move & Move::captureMask) >> Move::captureOffset;
        if (captured == Piece::create(Black, Pawn)) {
            Board::removePiece(board, to - 8ull);
            Board::setPiece(board, board.squares[from], to);
            Board::removePiece(board, from);
        } else {
            Board::removePiece(board, to + 8ull);
            Board::setPiece(board, board.squares[from], to);
            Board::removePiece(board, from);
        }
    } else {
        if (Move::isCapture(move)) Board::removePiece(board, to);
        if (Move::isPromotion(move)) {
            const auto promoted = (move & Move::promotionMask) >> Move::promotionOffset;
            Board::setPiece(board, promoted, to);
        } else {
            Board::setPiece(board, board.squares[from], to);
        }
        Board::removePiece(board, from);
    }

    Color::invert(board.turn);

    if (Move::isCastleLong(move)) {
        const Coord::Type rookFrom = (board.turn != White)? Coord::fromString("a1"): Coord::fromString("a8");
        const Coord::Type rookTo   = (board.turn != White)? Coord::fromString("d1"): Coord::fromString("d8");

        Board::setPiece(board, board.squares[rookFrom], rookTo);
        Board::removePiece(board, rookFrom);

        if (board.turn != White) {
            if (Checker::isAttacked<White>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, from)) return makeBoolspeed(0);
        } else {
            if (Checker::isAttacked<Black>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, from)) return makeBoolspeed(0);
        }
    } else if (Move::isCastleShort(move)) {
        const Coord::Type rookFrom = (board.turn != White)? Coord::fromString("h1"): Coord::fromString("h8");
        const Coord::Type rookTo   = (board.turn != White)? Coord::fromString("f1"): Coord::fromString("f8");

        Board::setPiece(board, board.squares[rookFrom], rookTo);
        Board::removePiece(board, rookFrom);

        if (board.turn != White) {
            if (Checker::isAttacked<White>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, from)) return makeBoolspeed(0);
        } else {
            if (Checker::isAttacked<Black>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, from)) return makeBoolspeed(0);
        }
    }

    return makeBoolspeed(1);
}

void Move::unmake(Type move, Board::Type& board) {
    Color::invert(board.turn);

    //@TODO: Coord::getFrom
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    //@TODO: Rewrite
    if (Move::isEnpassant(move)) {
        Piece::Type captured = (move & Move::captureMask) >> Move::captureOffset;
        if (captured == Piece::create(Black, Pawn)) {
            Board::setPiece(board, board.squares[to], from);
            Board::removePiece(board, to);
            Board::setPiece(board, captured, to - 8ull);
        } else {
            Board::setPiece(board, board.squares[to], from);
            Board::removePiece(board, to);
            Board::setPiece(board, captured, to + 8ull);
        }
    } else {
        if (Move::isPromotion(move)) {
            const auto promotedColor = ((move & Move::promotionMask) >> Move::promotionOffset) & Color::typeMask;
            Board::setPiece(board, Piece::create(promotedColor, Pawn), from);
        } else {
            Board::setPiece(board, board.squares[to], from);
        }
        Board::removePiece(board, to);
        //@TODO: Move::getCaptured
        if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
    }

    if (Move::isCastleLong(move)) {
        const Coord::Type rookFrom = (board.turn == White)? Coord::fromString("a1"): Coord::fromString("a8");
        const Coord::Type rookTo   = (board.turn == White)? Coord::fromString("d1"): Coord::fromString("d8");

        Board::setPiece(board, board.squares[rookTo], rookFrom);
        Board::removePiece(board, rookTo);
    } else if (Move::isCastleShort(move)) {
        const Coord::Type rookFrom = (board.turn == White)? Coord::fromString("h1"): Coord::fromString("h8");
        const Coord::Type rookTo   = (board.turn == White)? Coord::fromString("f1"): Coord::fromString("f8");

        Board::setPiece(board, board.squares[rookTo], rookFrom);
        Board::removePiece(board, rookTo);
    }

    ++board.depth;
}
