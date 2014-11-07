#include "captures.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

inline void addLegalCaptures(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type legals) {
    while(legals != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(legals);
        const auto to = Coord::Type(bitIndex);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to, board.squares[to]);

        legals ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void forKnightCapture(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type correctMask) {
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
        const auto bitIndex = Bitboard::bitScan(knights);

        forKnightCapture<COLOR>(buffer, board, Coord::Type(bitIndex), correctMask);

        knights ^= Bitboard::fromIndex(bitIndex);
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
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forBishopCapture<COLOR>(buffer, board, Coord::Type(bitIndex), correctMask);

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Captures::rooks(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forRookCapture<COLOR>(buffer, board, Coord::Type(bitIndex), correctMask);

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Captures::queens(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Queen)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forQueenCapture<COLOR>(buffer, board, Coord::Type(bitIndex), correctMask);

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

// Temporary solution, because gcc is stupid dick
namespace Captures {
template <> 
void pawns<White>(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    //@TODO(low): Refactoring?
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];

    UNumspeed bitIndex;

    auto workingBB = pawns & ~Bitboard::leftLine;
    workingBB <<= 8ull;
    workingBB >>= 1ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[Black];
    workingBB &= correctMask;
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);
        
        if (bitIndex >= 56ull) { //Pawn goes at last line
            ++buffer[0];
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex + 1ull - 8ull), to, Piece::create(White, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 1ull - 8ull), to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }

    workingBB = pawns & ~Bitboard::rightLine;
    workingBB <<= 8ull;
    workingBB <<= 1ull;
    auto rightCapturesEp = workingBB;
    workingBB &= board.bitboards[Black];
    workingBB &= correctMask;
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);
        
        if (bitIndex >= 56ull) { //Pawn goes at last line
            ++buffer[0];
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex - 1ull - 8ull), to, Piece::create(White, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 1ull - 8ull), to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        bitIndex = Board::enpassant(board);
        workingBB = Bitboard::fromIndex(bitIndex);
        const auto to = Coord::Type(bitIndex);

        leftCapturesEp &= workingBB;
        if (leftCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(Coord::Type(bitIndex + 1ull - 8ull), to);
        }

        rightCapturesEp &= workingBB;
        if (rightCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(Coord::Type(bitIndex - 1ull - 8ull), to);
        }
    }
}

template <> 
void pawns<Black>(Move::Buffer &buffer, const Board::Type &board, Bitboard::Type correctMask) {
    //@TODO: Refactoring?
    const auto pawns = board.bitboards[Piece::create(Black, Pawn)];

    UNumspeed bitIndex;
    auto workingBB = (pawns & ~Bitboard::leftLine) >> 9ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[White];
    workingBB &= correctMask;
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);

        const auto from = Coord::Type(bitIndex + 1ull + 8ull);
        const auto captured = board.squares[to];

        if (bitIndex < 8ull) { //Pawn goes at last line
            ++buffer[0];
            buffer[buffer[0]] = Move::promotion(from, to, Piece::create(Black, Queen), captured);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, to, captured);
        }

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }

    workingBB = (pawns & ~Bitboard::rightLine) >> 7ull;
    auto rightCapturesEp = workingBB;
    workingBB &= board.bitboards[White];
    workingBB &= correctMask;
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);

        const auto from = Coord::Type(bitIndex - 1ull + 8ull);
        const auto captured = board.squares[to];
        
        if (bitIndex < 8ull) { //Pawn goes at last line
            ++buffer[0];
            buffer[buffer[0]] = Move::promotion(from, to, Piece::create(Black, Queen), captured);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, to, captured);
        }

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        bitIndex = Board::enpassant(board);
        workingBB = Bitboard::fromIndex(bitIndex);
        const auto to = Coord::Type(bitIndex);

        leftCapturesEp &= workingBB;
        if (leftCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(Coord::Type(bitIndex + 1ull + 8ull), to);
        }

        rightCapturesEp &= workingBB;
        if (rightCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(Coord::Type(bitIndex - 1ull + 8ull), to);
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
