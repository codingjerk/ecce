#include "evaluation.hpp"

using namespace Eval;

namespace Eval {
template <>
Score::Type material<White>(const Board::Type& board) {
    Score::Type result = 0;

    result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Pawn)])   * Score::Pawn;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Knight)]) * Score::Knight;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Bishop)]) * Score::Bishop;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Rook)])   * Score::Rook;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(White, Queen)])  * Score::Queen;
    
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Pawn)])   * Score::Pawn;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Knight)]) * Score::Knight;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Bishop)]) * Score::Bishop;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Rook)])   * Score::Rook;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(Black, Queen)])  * Score::Queen;

    return result;
}

template <>
Score::Type material<Black>(const Board::Type& board) {
    Score::Type result = 0;
    
    result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Pawn)])   * Score::Pawn;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Knight)]) * Score::Knight;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Bishop)]) * Score::Bishop;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Rook)])   * Score::Rook;
    result += Bitboard::enabledCount(board.bitboards[Piece::create(Black, Queen)])  * Score::Queen;

    result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Pawn)])   * Score::Pawn;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Knight)]) * Score::Knight;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Bishop)]) * Score::Bishop;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Rook)])   * Score::Rook;
    result -= Bitboard::enabledCount(board.bitboards[Piece::create(White, Queen)])  * Score::Queen;

    return result;
}
}

template Score::Type Eval::material<White>(const Board::Type&);
template Score::Type Eval::material<Black>(const Board::Type&);
