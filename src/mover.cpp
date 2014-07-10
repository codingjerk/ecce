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

    if (Move::isCapture(move)) Board::removePiece(board, to);
    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);

    return makeBoolspeed(1);
}

void Move::unmake(Type move, Board::Type& board) {
    Color::invert(board.turn);

    //@TODO: Coord::getFrom
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);
    //@TODO: Move::getCaptured
    if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);

    ++board.depth;
}
