#include "captures.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

inline void addLegalCaptures(Move::Buffer &buffer, const Board::Type &board, const Coord::Type &from, Bitboard::Type legals) {
    while(legals != Bitboard::null) {
        Coord::Type to = Bitboard::bitScan(legals);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to, board.squares[to]);

        legals ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR>
void forKnightCapture(Move::Buffer &buffer, const Board::Type &board, const Coord::Type &from, Bitboard::Type correctMask) {
    MAKEOPP(COLOR);
    const Bitboard::Type legalSquares = (board.bitboards[OPP]) & Tables::knightMasks[from] & correctMask;

    addLegalCaptures(buffer, board, from, legalSquares);
}

template <Color::Type COLOR>
void forKingCapture(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type correctMask) {
    MAKEOPP(COLOR);
    const Bitboard::Type legalSquares = (board.bitboards[OPP]) & Tables::kingMasks[from] & correctMask;

    addLegalCaptures(buffer, board, from, legalSquares);
}

template <Color::Type COLOR>
void forBishopCapture(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type correctMask) {
    MAKEOPP(COLOR);
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) * Magic::bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegalCaptures(buffer, board, from, Magic::bishopData[magicIndex] & board.bitboards[OPP] & correctMask);
}

template <Color::Type COLOR>
void forRookCapture(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type correctMask) {
    MAKEOPP(COLOR);
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    addLegalCaptures(buffer, board, from, Magic::rookData[magicIndex] & board.bitboards[OPP] & correctMask);
}

template <Color::Type COLOR>
void forQueenCapture(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type correctMask) {
    MAKEOPP(COLOR);
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed rookMagicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    const UNumspeed bishopMagicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) *Magic:: bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegalCaptures(buffer, board, from, (Magic::rookData[rookMagicIndex] | Magic::bishopData[bishopMagicIndex]) & board.bitboards[OPP] & correctMask);
}

template <Color::Type COLOR>
void Captures::knights(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != Bitboard::null) {
        Coord::Type to = Bitboard::bitScan(knights);

        forKnightCapture<COLOR>(buffer, board, to, correctMask);

        knights ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR>
void Captures::kings(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    auto bitboard = board.bitboards[Piece::create(COLOR, King)];

    forKingCapture<COLOR>(buffer, board, Coord::Type(Bitboard::bitScan(bitboard)), correctMask);
}

template <Color::Type COLOR> 
void Captures::bishops(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != Bitboard::null) {
        Coord::Type to = Bitboard::bitScan(bitboard);

        forBishopCapture<COLOR>(buffer, board, to, correctMask);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR> 
void Captures::rooks(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != Bitboard::null) {
        Coord::Type to = Bitboard::bitScan(bitboard);

        forRookCapture<COLOR>(buffer, board, to, correctMask);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR> 
void Captures::queens(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Queen)];
    while(bitboard != Bitboard::null) {
        Coord::Type to = Bitboard::bitScan(bitboard);

        forQueenCapture<COLOR>(buffer, board, to, correctMask);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

// Temporary solution, because gcc is stupid dick
namespace Captures {
template <> 
void pawns<White>(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];

    Coord::Type to;
    auto workingBB = (pawns & ~Bitboard::leftLine) << 7ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[Black] & correctMask;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        ++buffer[0];
        if (to >= 56ull) { //Pawn goes at last line
            buffer[buffer[0]] = Move::promotion(to - 7ull, to, Piece::create(White, Queen), board.squares[to]);
        } else {
            buffer[buffer[0]] = Move::create(to - 7ull, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = (pawns & ~Bitboard::rightLine) << 9ull;
    auto rightCapturesEp = workingBB;
    workingBB &= board.bitboards[Black] & correctMask;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        if (to >= 56ull) { //Pawn goes at last line
            ++buffer[0];
            buffer[buffer[0]] = Move::promotion(to - 9ull, to, Piece::create(White, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(to - 9ull, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        to = Board::enpassant(board);
        workingBB = Bitboard::fromCoord(to);

        leftCapturesEp &= workingBB;
        if (leftCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(to - 7ull, to);
        }

        rightCapturesEp &= workingBB;
        if (rightCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(to - 9ull, to);
        }
    }
}

template <> 
void pawns<Black>(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    const auto pawns = board.bitboards[Piece::create(Black, Pawn)];

    Coord::Type to;
    auto workingBB = (pawns & ~Bitboard::leftLine) >> 9ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[White] & correctMask;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        const auto from = to + 9ull;

        ++buffer[0];
        if (to < 8ull) { //Pawn goes at last line
            buffer[buffer[0]] = Move::promotion(from, to, Piece::create(Black, Queen), board.squares[to]);
        } else {
            buffer[buffer[0]] = Move::create(from, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = (pawns & ~Bitboard::rightLine) >> 7ull;
    auto rightCapturesEp = workingBB;
    workingBB &= board.bitboards[White] & correctMask;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        const auto from = to + 7ull;

        ++buffer[0];
        if (to < 8ull) { //Pawn goes at last line
            buffer[buffer[0]] = Move::promotion(from, to, Piece::create(Black, Queen), board.squares[to]);
        } else {
            buffer[buffer[0]] = Move::create(from, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        to = Board::enpassant(board);
        workingBB = Bitboard::fromCoord(to);

        leftCapturesEp &= workingBB;
        if (leftCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(to + 9ull, to);
        }

        rightCapturesEp &= workingBB;
        if (rightCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(to + 7ull, to);
        }
    }
}
}

inline void mvvSort(Move::Buffer &buffer, Move::Type start, Move::Type end) {
    if (start >= end) return;

    for (auto i = start; i <= end; ++i) {
        for (auto j = i; (j > start) && (buffer[j] > buffer[j - 1]); --j) {
            std::swap(buffer[j], buffer[j - 1]);
        }
    }
}

#define SORTED(command) \
    start = buffer[0]; \
    command<COLOR>(buffer, board, correctMask); \
    end = buffer[0]; \
    mvvSort(buffer, start + 1, end);

template <Color::Type COLOR> 
void Captures::phase(Move::Buffer &buffer, const Board::Type &board) {
    buffer[0] = 0;
    
    Move::Type start, end;

    auto const correctMask = ~Bitboard::fromCoord(board.depthPtr->lastMoved);
    
    SORTED(kings);
    SORTED(pawns);
    SORTED(knights);
    SORTED(bishops);
    SORTED(rooks);
    SORTED(queens);
}

// Explicit template instantiations
template void Captures::phase<White>(Move::Buffer&, const Board::Type&);
template void Captures::phase<Black>(Move::Buffer&, const Board::Type&);
