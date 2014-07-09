#include "mover.hpp"

#include <iostream>

//@TODO(FAST): Make it template
Boolspeed Move::make(Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);

    return makeBoolspeed(1);
}

void Move::unmake(Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);
}
