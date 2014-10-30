#include "silents.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"
#include "history.hpp"
#include "killers.hpp"

// @TODO: Move to generator as public
inline void addLegalsSilent(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type legals) {
    while(legals != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(legals);
        const auto to = Coord::Type(bitIndex);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to);

        legals ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void forKnightSilent(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~(board.bitboards[White] | board.bitboards[Black])) & Tables::knightMasks[from];

    addLegalsSilent(buffer, board, from, legalSquares);
}

template <Color::Type COLOR>
void forKingSilent(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    MAKEOPP(COLOR);
    const Bitboard::Type legal = ~(board.bitboards[White] | board.bitboards[Black]);
    const Bitboard::Type legalSquares = (legal) & Tables::kingMasks[from];

    addLegalsSilent(buffer, board, from, legalSquares);

    if (Castle::is<COLOR, King>(Board::castle(board))) {
        if ((Tables::castleNeeded[COLOR][King] & legal) == Tables::castleNeeded[COLOR][King]) {
            ++buffer[0];
            buffer[buffer[0]] = Move::castleShort<COLOR>();
        }
    }

    if (Castle::is<COLOR, Queen>(Board::castle(board))) {
        if ((Tables::castleNeeded[COLOR][Queen] & legal) == Tables::castleNeeded[COLOR][Queen]) {
            ++buffer[0];
            buffer[buffer[0]] = Move::castleLong<COLOR>();
        }
    }
}

template <Color::Type COLOR>
void forBishopSilent(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) * Magic::bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegalsSilent(buffer, board, from, Magic::bishopData[magicIndex] & (~nonEmpty));
}

template <Color::Type COLOR>
void forRookSilent(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    addLegalsSilent(buffer, board, from, Magic::rookData[magicIndex] & (~nonEmpty));
}

template <Color::Type COLOR>
void forQueenSilent(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed rookMagicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    const UNumspeed bishopMagicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) *Magic:: bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegalsSilent(buffer, board, from, (Magic::rookData[rookMagicIndex] | Magic::bishopData[bishopMagicIndex]) & (~nonEmpty));
}

template <Color::Type COLOR>
void Silents::knights(Move::Buffer &buffer, const Board::Type &board) {
    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(knights);

        forKnightSilent<COLOR>(buffer, board, Coord::Type(bitIndex));

        knights ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void Silents::kings(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, King)];

    forKingSilent<COLOR>(buffer, board, Coord::Type(Bitboard::bitScan(bitboard)));
}

template <Color::Type COLOR> 
void Silents::bishops(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forBishopSilent<COLOR>(buffer, board, Coord::Type(bitIndex));
        
        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Silents::rooks(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forRookSilent<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Silents::queens(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Queen)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forQueenSilent<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

// Temporary solution, because gcc is stupid dick
namespace Silents {
template <> 
void pawns<White>(Move::Buffer &buffer, const Board::Type &board) {
    //@TODO(low): Refactoring?
    const Bitboard::Type legalSquares = ~(board.bitboards[White] | board.bitboards[Black]);
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];
    const auto onestep = (pawns << makeUNum64(8)) & legalSquares;

    UNumspeed bitIndex;
    auto workingBB = onestep;
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        
        if (bitIndex >= 56ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex - 8ull), Coord::Type(bitIndex), Piece::create(White, Knight));
            buffer[buffer[0]-1] = Move::promotion(Coord::Type(bitIndex - 8ull), Coord::Type(bitIndex), Piece::create(White, Bishop));
            buffer[buffer[0]-2] = Move::promotion(Coord::Type(bitIndex - 8ull), Coord::Type(bitIndex), Piece::create(White, Rook));
            buffer[buffer[0]-3] = Move::promotion(Coord::Type(bitIndex - 8ull), Coord::Type(bitIndex), Piece::create(White, Queen));
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 8ull), Coord::Type(bitIndex));
        }

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }

    workingBB = pawns & Tables::pawnStartLine[White];
    workingBB &= onestep >> 8ull;
    workingBB = (workingBB << 16ull) & legalSquares; 
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(Coord::Type(bitIndex - 16ull), Coord::Type(bitIndex));

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }
}

template <> 
void pawns<Black>(Move::Buffer &buffer, const Board::Type &board) {
    //@TODO: Refactoring?
    const Bitboard::Type legalSquares = ~(board.bitboards[Black] | board.bitboards[White]);
    const auto pawns = board.bitboards[Piece::create(Black, Pawn)];
    const auto onestep = (pawns >> makeUNum64(8)) & legalSquares;

    UNumspeed bitIndex;
    auto workingBB = onestep;
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);

        const auto from = Coord::Type(bitIndex + 8ull);
        
        if (bitIndex < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(from, Coord::Type(bitIndex), Piece::create(Black, Knight));
            buffer[buffer[0]-1] = Move::promotion(from, Coord::Type(bitIndex), Piece::create(Black, Bishop));
            buffer[buffer[0]-2] = Move::promotion(from, Coord::Type(bitIndex), Piece::create(Black, Rook));
            buffer[buffer[0]-3] = Move::promotion(from, Coord::Type(bitIndex), Piece::create(Black, Queen));
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, Coord::Type(bitIndex));
        }
        
        workingBB ^= Bitboard::fromIndex(bitIndex);
    }

    workingBB = pawns & Tables::pawnStartLine[Black];
    workingBB &= onestep << 8ull;
    workingBB = (workingBB >> 16ull) & legalSquares; 
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);

        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(Coord::Type(bitIndex + 16ull), Coord::Type(bitIndex));

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }
}
}

inline void historySortInsertion(Move::Buffer &buffer, Move::Type start, Move::Type end) {
    if (start >= end) return;

    for (auto i = start; i <= end; ++i) {
        for (auto j = i; (j > start) && (History::score(buffer[j]) > History::score(buffer[j - 1])); --j) {
            std::swap(buffer[j], buffer[j - 1]);
        }
    }
}

void historyCheck(Move::Buffer &buffer, Move::Type start, Move::Type end) {
    for (auto i = start; i < end; ++i) {
        if (History::score(buffer[i]) < History::score(buffer[i+1])) {
            std::cout << "Error!\n";
        }
    }
}

inline Move::Type upKillers(Move::Buffer &buffer, const Board::Type &board) {
    Move::Type offset = 1;

    for (auto i = 1; i <= buffer[0]; ++i) {
        if (buffer[i] == Killer::first(board) || buffer[i] == Killer::second(board)) {
            std::swap(buffer[i], buffer[offset]);
            ++offset;
            if (offset == 3) break;
        }
    }

    return offset;
}

template <Color::Type COLOR> 
void Silents::phase(Move::Buffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    queens<COLOR>(buffer, board);
    rooks<COLOR>(buffer, board);
    knights<COLOR>(buffer, board);
    bishops<COLOR>(buffer, board);
    pawns<COLOR>(buffer, board);
    kings<COLOR>(buffer, board);
    
    auto offset = upKillers(buffer, board);
    historySortInsertion(buffer, offset, buffer[0]);
}

// Explicit template instantiations
template void Silents::knights<White>(Move::Buffer&, const Board::Type&);
template void Silents::knights<Black>(Move::Buffer&, const Board::Type&);

template void Silents::kings<White>(Move::Buffer&, const Board::Type&);
template void Silents::kings<Black>(Move::Buffer&, const Board::Type&);

template void Silents::bishops<White>(Move::Buffer&, const Board::Type&);
template void Silents::bishops<Black>(Move::Buffer&, const Board::Type&);

template void Silents::rooks<White>(Move::Buffer&, const Board::Type&);
template void Silents::rooks<Black>(Move::Buffer&, const Board::Type&);

template void Silents::queens<White>(Move::Buffer&, const Board::Type&);
template void Silents::queens<Black>(Move::Buffer&, const Board::Type&);

template void Silents::phase<White>(Move::Buffer&, const Board::Type&);
template void Silents::phase<Black>(Move::Buffer&, const Board::Type&);
