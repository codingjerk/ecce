#include "mover.hpp"

#include "generatorTables.hpp"

#include <iostream>

//@TODO(FAST): Make it template
Boolspeed Move::make(Type move, Board::Type& board) {
    Color::invert(board.turn);

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

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
        Board::setPiece(board, board.squares[from], to);
        Board::removePiece(board, from);
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
        Board::setPiece(board, board.squares[to], from);
        Board::removePiece(board, to);
        //@TODO: Move::getCaptured
        if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
    }

    ++board.depth;
}
