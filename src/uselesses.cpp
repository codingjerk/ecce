#include "uselesses.hpp"

#include "generatorTables.hpp"
#include "magics.hpp"

template <Color::Type COLOR>
void uselessesPromotions(Move::Buffer &buffer, const Board::Type &board);

template <>
void uselessesPromotions<White>(Move::Buffer &buffer, const Board::Type &board) {
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];

    Coord::Type to;

    auto workingBB = pawns & Tables::pawnSubPromotionLine[White] & ~Bitboard::leftLine;
    workingBB <<= 7ull;
    workingBB &= board.bitboards[Black];
    while (workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        buffer[0] += 3;
        buffer[buffer[0]] = Move::promotion(to - 7ull, to, Piece::create(White, Knight), board.squares[to]);
        buffer[buffer[0] - 1] = Move::promotion(to - 7ull, to, Piece::create(White, Bishop), board.squares[to]);
        buffer[buffer[0] - 2] = Move::promotion(to - 7ull, to, Piece::create(White, Rook), board.squares[to]);

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = pawns & Tables::pawnSubPromotionLine[White] & ~Bitboard::rightLine;
    workingBB <<= 9ull;
    workingBB &= board.bitboards[Black];
    while (workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        buffer[0] += 3;
        buffer[buffer[0]] = Move::promotion(to - 9ull, to, Piece::create(White, Knight), board.squares[to]);
        buffer[buffer[0] - 1] = Move::promotion(to - 9ull, to, Piece::create(White, Bishop), board.squares[to]);
        buffer[buffer[0] - 2] = Move::promotion(to - 9ull, to, Piece::create(White, Rook), board.squares[to]);

        workingBB ^= Bitboard::fromCoord(to);
    }

    const Bitboard::Type legalSquares = ~(board.bitboards[White] | board.bitboards[Black]);
    const auto onestep = ((pawns & Tables::pawnSubPromotionLine[White]) << makeUNum64(8)) & legalSquares;
    workingBB = onestep;
    while (workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        buffer[0] += 3;
        buffer[buffer[0]] = Move::promotion(to - 8ull, to, Piece::create(White, Knight));
        buffer[buffer[0] - 1] = Move::promotion(to - 8ull, to, Piece::create(White, Bishop));
        buffer[buffer[0] - 2] = Move::promotion(to - 8ull, to, Piece::create(White, Rook));

        workingBB ^= Bitboard::fromCoord(to);
    }
}

template <>
void uselessesPromotions<Black>(Move::Buffer &buffer, const Board::Type &board) {
    const auto pawns = board.bitboards[Piece::create(Black, Pawn)];

    Coord::Type to;
    auto workingBB = (pawns & Tables::pawnSubPromotionLine[Black] & ~Bitboard::leftLine) >> 9ull;
    workingBB &= board.bitboards[White];
    while (workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        buffer[0] += 3;
        buffer[buffer[0]] = Move::promotion(to + 9ull, to, Piece::create(Black, Knight), board.squares[to]);
        buffer[buffer[0] - 1] = Move::promotion(to + 9ull, to, Piece::create(Black, Bishop), board.squares[to]);
        buffer[buffer[0] - 2] = Move::promotion(to + 9ull, to, Piece::create(Black, Rook), board.squares[to]);

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = (pawns & Tables::pawnSubPromotionLine[Black] & ~Bitboard::rightLine) >> 7ull;
    workingBB &= board.bitboards[White];
    while (workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        buffer[0] += 3;
        buffer[buffer[0]] = Move::promotion(to + 7ull, to, Piece::create(Black, Knight), board.squares[to]);
        buffer[buffer[0] - 1] = Move::promotion(to + 7ull, to, Piece::create(Black, Bishop), board.squares[to]);
        buffer[buffer[0] - 2] = Move::promotion(to + 7ull, to, Piece::create(Black, Rook), board.squares[to]);

        workingBB ^= Bitboard::fromCoord(to);
    }

    const Bitboard::Type legalSquares = ~(board.bitboards[Black] | board.bitboards[White]);
    const auto onestep = ((pawns & Tables::pawnSubPromotionLine[Black]) >> makeUNum64(8)) & legalSquares;
    workingBB = onestep;
    while (workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        buffer[0] += 3;
        buffer[buffer[0]] = Move::promotion(to + 8ull, to, Piece::create(Black, Knight));
        buffer[buffer[0] - 1] = Move::promotion(to + 8ull, to, Piece::create(Black, Bishop));
        buffer[buffer[0] - 2] = Move::promotion(to + 8ull, to, Piece::create(Black, Rook));

        workingBB ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR>
void Uselesses::phase(Move::Buffer& buffer, const Board::Type& board) {
    buffer[0] = 0;

    uselessesPromotions<COLOR>(buffer, board);
}

template void Uselesses::phase<White>(Move::Buffer&, const Board::Type&);
template void Uselesses::phase<Black>(Move::Buffer&, const Board::Type&);
