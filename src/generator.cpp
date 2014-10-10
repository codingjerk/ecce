#include "generator.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

using namespace Generator;

inline void addLegals(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type legals) {
    while(legals != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(legals);
        const auto to = Coord::Type(bitIndex);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to, board.squares[to]);

        legals ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void forKnight(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[COLOR]) & Tables::knightMasks[from];

    addLegals(buffer, board, from, legalSquares);
}

template <Color::Type COLOR>
void forKing(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[COLOR]) & Tables::kingMasks[from];

    addLegals(buffer, board, from, legalSquares);

    const Bitboard::Type legal = ~(board.bitboards[White] | board.bitboards[Black]);

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
void forBishop(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) * Magic::bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegals(buffer, board, from, Magic::bishopData[magicIndex] & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void forRook(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    addLegals(buffer, board, from, Magic::rookData[magicIndex] & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void forQueen(Move::Buffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed rookMagicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    const UNumspeed bishopMagicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) *Magic:: bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegals(buffer, board, from, (Magic::rookData[rookMagicIndex] | Magic::bishopData[bishopMagicIndex]) & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void Generator::forKnights(Move::Buffer &buffer, const Board::Type &board) {
    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(knights);

        forKnight<COLOR>(buffer, board, Coord::Type(bitIndex));

        knights ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void Generator::forKings(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, King)];

    forKing<COLOR>(buffer, board, Coord::Type(Bitboard::bitScan(bitboard)));
}

template <Color::Type COLOR> 
void Generator::forBishops(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forBishop<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forRooks(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forRook<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forQueens(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Queen)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forQueen<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

// Temporary solution, because gcc is stupid dick
namespace Generator {
template <> 
void forPawns<White>(Move::Buffer &buffer, const Board::Type &board) {
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

    workingBB = pawns & ~Bitboard::leftLine;
    workingBB <<= 8ull;
    workingBB >>= 1ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[Black];
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);
        
        if (bitIndex >= 56ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex + 1ull - 8ull), to, Piece::create(White, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(Coord::Type(bitIndex + 1ull - 8ull), to, Piece::create(White, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(Coord::Type(bitIndex + 1ull - 8ull), to, Piece::create(White, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(Coord::Type(bitIndex + 1ull - 8ull), to, Piece::create(White, Queen), board.squares[to]);
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
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);
        
        if (bitIndex >= 56ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex - 1ull - 8ull), to, Piece::create(White, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(Coord::Type(bitIndex - 1ull - 8ull), to, Piece::create(White, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(Coord::Type(bitIndex - 1ull - 8ull), to, Piece::create(White, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(Coord::Type(bitIndex - 1ull - 8ull), to, Piece::create(White, Queen), board.squares[to]);
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
void forPawns<Black>(Move::Buffer &buffer, const Board::Type &board) {
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

    workingBB = (pawns & ~Bitboard::leftLine) >> 9ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[White];
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);

        const auto from = Coord::Type(bitIndex + 1ull + 8ull);
        const auto captured = board.squares[to];

        if (bitIndex < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(from, to, Piece::create(Black, Knight), captured);
            buffer[buffer[0]-1] = Move::promotion(from, to, Piece::create(Black, Bishop), captured);
            buffer[buffer[0]-2] = Move::promotion(from, to, Piece::create(Black, Rook), captured);
            buffer[buffer[0]-3] = Move::promotion(from, to, Piece::create(Black, Queen), captured);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, to, captured);
        }

        workingBB ^= Bitboard::fromIndex(bitIndex);
    }

    workingBB = (pawns & ~Bitboard::rightLine) >> 7ull;
    auto rightCapturesEp = workingBB;
    workingBB &= board.bitboards[White];
    while(workingBB != Bitboard::null) {
        bitIndex = Bitboard::bitScan(workingBB);
        const auto to = Coord::Type(bitIndex);

        const auto from = Coord::Type(bitIndex - 1ull + 8ull);
        const auto captured = board.squares[to];
        
        if (bitIndex < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(from, to, Piece::create(Black, Knight), captured);
            buffer[buffer[0]-1] = Move::promotion(from, to, Piece::create(Black, Bishop), captured);
            buffer[buffer[0]-2] = Move::promotion(from, to, Piece::create(Black, Rook), captured);
            buffer[buffer[0]-3] = Move::promotion(from, to, Piece::create(Black, Queen), captured);
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

template <Color::Type COLOR> 
void Generator::forBoard(Move::Buffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    forPawns<COLOR>(buffer, board);
    forKnights<COLOR>(buffer, board);
    forBishops<COLOR>(buffer, board);
    forRooks<COLOR>(buffer, board);
    forKings<COLOR>(buffer, board);
    forQueens<COLOR>(buffer, board);
}

void Generator::forBoard(Move::Buffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forBoard<White>(buffer, board);
    } else {
        forBoard<Black>(buffer, board);
    }
}

// Explicit template instantiations
template void Generator::forKnights<White>(Move::Buffer&, const Board::Type&);
template void Generator::forKnights<Black>(Move::Buffer&, const Board::Type&);

template void Generator::forKings<White>(Move::Buffer&, const Board::Type&);
template void Generator::forKings<Black>(Move::Buffer&, const Board::Type&);

template void Generator::forBishops<White>(Move::Buffer&, const Board::Type&);
template void Generator::forBishops<Black>(Move::Buffer&, const Board::Type&);

template void Generator::forRooks<White>(Move::Buffer&, const Board::Type&);
template void Generator::forRooks<Black>(Move::Buffer&, const Board::Type&);

template void Generator::forQueens<White>(Move::Buffer&, const Board::Type&);
template void Generator::forQueens<Black>(Move::Buffer&, const Board::Type&);

template void Generator::forBoard<White>(Move::Buffer&, const Board::Type&);
template void Generator::forBoard<Black>(Move::Buffer&, const Board::Type&);

void Generator::initTables() {
    Tables::initTables();
    Magic::initTables();
}
