#include "silents.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"
#include "history.hpp"
#include "killers.hpp"

inline void addLegalsSilent(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type legals) {
    while(legals != Bitboard::null) {
        const auto to = Bitboard::bitScan(legals);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to);

        legals ^= Bitboard::fromCoord(to);
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
void knights(Move::Buffer &buffer, const Board::Type &board) {
    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != Bitboard::null) {
        const auto to = Bitboard::bitScan(knights);

        forKnightSilent<COLOR>(buffer, board, to);

        knights ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR>
void kings(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, King)];

    forKingSilent<COLOR>(buffer, board, Coord::Type(Bitboard::bitScan(bitboard)));
}

template <Color::Type COLOR> 
void bishops(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != Bitboard::null) {
        const auto to = Bitboard::bitScan(bitboard);

        forBishopSilent<COLOR>(buffer, board, to);
        
        bitboard ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR> 
void rooks(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != Bitboard::null) {
        const auto to = Bitboard::bitScan(bitboard);

        forRookSilent<COLOR>(buffer, board, to);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR> 
void queens(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Queen)];
    while(bitboard != Bitboard::null) {
        const auto to = Bitboard::bitScan(bitboard);

        forQueenSilent<COLOR>(buffer, board, to);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR>
void pawns(Move::Buffer &buffer, const Board::Type &board);

template <> 
void pawns<White>(Move::Buffer &buffer, const Board::Type &board) {
    const Bitboard::Type legalSquares = ~(board.bitboards[White] | board.bitboards[Black]);
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];
    const auto onestep = (pawns << makeUNum64(8)) & legalSquares;

    Coord::Type to;
    auto workingBB = onestep;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        ++buffer[0];
        if (to >= 56ull) { //Pawn goes at last line
            buffer[buffer[0]] = Move::promotion(to - 8ull, to, Piece::create(White, Queen));
        } else {
            buffer[buffer[0]] = Move::create(to - 8ull, to);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = pawns & Tables::pawnStartLine[White];
    workingBB &= onestep >> 8ull;
    workingBB = (workingBB << 16ull) & legalSquares; 
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(to - 16ull, to);

        workingBB ^= Bitboard::fromCoord(to);
    }
}

template <> 
void pawns<Black>(Move::Buffer &buffer, const Board::Type &board) {
    const Bitboard::Type legalSquares = ~(board.bitboards[Black] | board.bitboards[White]);
    const auto pawns = board.bitboards[Piece::create(Black, Pawn)];
    const auto onestep = (pawns >> makeUNum64(8)) & legalSquares;

    Coord::Type to;
    auto workingBB = onestep;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        ++buffer[0];
        if (to < 8ull) { //Pawn goes at last line
            buffer[buffer[0]] = Move::promotion(to + 8ull, to, Piece::create(Black, Queen));
        } else {
            buffer[buffer[0]] = Move::create(to + 8ull, to);
        }
        
        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = pawns & Tables::pawnStartLine[Black] & (onestep << 8ull);
    workingBB = (workingBB >> 16ull) & legalSquares; 
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(to + 16ull, to);

        workingBB ^= Bitboard::fromCoord(to);
    }
}

inline void historySort(Move::Buffer &buffer, Move::Type start, Move::Type end) {
    if (start >= end) return;

    for (auto i = start; i <= end; ++i) {
        for (auto j = i; (j > start) && (History::score(buffer[j]) > History::score(buffer[j - 1])); --j) {
            std::swap(buffer[j], buffer[j - 1]);
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
    historySort(buffer, offset, buffer[0]);
}

// Explicit template instantiations
template void Silents::phase<White>(Move::Buffer&, const Board::Type&);
template void Silents::phase<Black>(Move::Buffer&, const Board::Type&);
