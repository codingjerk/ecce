#include "mover.hpp"

#include <iostream>

UNumspeed Move::make(Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    std::cout << "Move " << Move::show(move) << ".\n";
    std::cout << "Making move from " << Coord::show(from) << " to " << Coord::show(to) << ".\n";
    std::cout << "I will move piece " << Piece::show(board.squares[from]) << ".\n";
    std::cout << "I will capture piece " << Piece::show(board.squares[to]) << ".\n";

    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);

    return fail;
}

void Move::unmake(Type move, Board::Type&) {

}
