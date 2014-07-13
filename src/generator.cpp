//@TODO(TRY): Try use precalculated move buffers for knights, kings and pawns
            // knightMovingIndex = ... magic ...
            // additionalBuffer = knightMoves[from][knightMovingIndex];
            // Buffer::add(buffer, additionalBuffer)

#include "generator.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

using namespace Generator;

inline void addLegals(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from, Bitboard::Type legals) {
    while(legals != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(legals);
        const auto to = Coord::Type(bitIndex);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to, board.squares[to]);

        legals ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void forKnight(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[COLOR]) & Tables::knightMasks[from];

    addLegals(buffer, board, from, legalSquares);
}

template <Color::Type COLOR>
void forKing(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
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
void forBishop(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) * Magic::bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegals(buffer, board, from, Magic::bishopData[magicIndex] & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void forRook(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    addLegals(buffer, board, from, Magic::rookData[magicIndex] & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void forQueen(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
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
void Generator::forKnights(MoveBuffer &buffer, const Board::Type &board) {
    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(knights);

        forKnight<COLOR>(buffer, board, Coord::Type(bitIndex));

        knights ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void Generator::forKings(MoveBuffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, King)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forKing<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forBishops(MoveBuffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forBishop<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forRooks(MoveBuffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forRook<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forQueens(MoveBuffer &buffer, const Board::Type &board) {
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
void forPawns<White>(MoveBuffer &buffer, const Board::Type &board) {
    //@TODO(low): Refactoring using tables?
    const Bitboard::Type legalSquares = ~(board.bitboards[White] | board.bitboards[Black]);
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];
    const auto onestep = (pawns << makeUNum64(8)) & legalSquares;

    UNumspeed bitIndex;
    auto legals = onestep;
    while(legals != Bitboard::null) {
        bitIndex = Bitboard::bitScan(legals);
        
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

        legals ^= Bitboard::fromIndex(bitIndex);
    }

    auto twosteps = pawns & Tables::pawnStartLine[White];
    twosteps &= onestep >> 8ull;
    twosteps = (twosteps << 16ull) & legalSquares; 
    while(twosteps != Bitboard::null) {
        bitIndex = Bitboard::bitScan(twosteps);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(Coord::Type(bitIndex - 16ull), Coord::Type(bitIndex));

        twosteps ^= Bitboard::fromIndex(bitIndex);
    }

    auto leftCaptures = pawns & ~Bitboard::leftLine;
    leftCaptures <<= 8ull;
    leftCaptures >>= 1ull;
    auto leftCapturesEp = leftCaptures;
    leftCaptures &= board.bitboards[Black];
    while(leftCaptures != Bitboard::null) {
        bitIndex = Bitboard::bitScan(leftCaptures);
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

        leftCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    auto rightCaptures = pawns & ~Bitboard::rightLine;
    rightCaptures <<= 8ull;
    rightCaptures <<= 1ull;
    auto rightCapturesEp = rightCaptures;
    rightCaptures &= board.bitboards[Black];
    while(rightCaptures != Bitboard::null) {
        bitIndex = Bitboard::bitScan(rightCaptures);
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

        rightCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        const auto bitIndex = Board::enpassant(board);
        Bitboard::Type enpassantBitboard = Bitboard::fromIndex(bitIndex);
        const auto to = Coord::Type(bitIndex);

        leftCapturesEp &= enpassantBitboard;
        if (leftCapturesEp != Bitboard::null) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(Coord::Type(bitIndex + 1ull - 8ull), to);
        }

        rightCapturesEp &= enpassantBitboard;
        if (rightCapturesEp != Bitboard::null) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(Coord::Type(bitIndex - 1ull - 8ull), to);
        }
    }
}

template <> 
void forPawns<Black>(MoveBuffer &buffer, const Board::Type &board) {
    //@TODO: Refactoring using tables?
    const Bitboard::Type legalSquares = ~(board.bitboards[Black] | board.bitboards[White]);
    const auto pawns = board.bitboards[Piece::create(Black, Pawn)];
    const auto onestep = (pawns >> makeUNum64(8)) & legalSquares;

    auto legals = onestep;
    while(legals != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(legals);

        if (bitIndex < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex + 8ull), Coord::Type(bitIndex), Piece::create(Black, Knight));
            buffer[buffer[0]-1] = Move::promotion(Coord::Type(bitIndex + 8ull), Coord::Type(bitIndex), Piece::create(Black, Bishop));
            buffer[buffer[0]-2] = Move::promotion(Coord::Type(bitIndex + 8ull), Coord::Type(bitIndex), Piece::create(Black, Rook));
            buffer[buffer[0]-3] = Move::promotion(Coord::Type(bitIndex + 8ull), Coord::Type(bitIndex), Piece::create(Black, Queen));
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 8ull), Coord::Type(bitIndex));
        }
        
        legals ^= Bitboard::fromIndex(bitIndex);
    }

    const auto startpawns = pawns & Tables::pawnStartLine[Black];
    auto twosteps = startpawns;
    twosteps &= onestep << 8ull;
    twosteps = (twosteps >> 16ull) & legalSquares; 
    while(twosteps != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(twosteps);

        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(Coord::Type(bitIndex + 16ull), Coord::Type(bitIndex));

        twosteps ^= Bitboard::fromIndex(bitIndex);
    }

    auto leftCaptures = pawns & ~Bitboard::leftLine;
    leftCaptures >>= 8ull;
    leftCaptures >>= 1ull;
    auto leftCapturesEp = leftCaptures;
    leftCaptures &= board.bitboards[White];
    while(leftCaptures != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(leftCaptures);
        const auto to = Coord::Type(bitIndex);

        if (bitIndex < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex + 1ull + 8ull), to, Piece::create(Black, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(Coord::Type(bitIndex + 1ull + 8ull), to, Piece::create(Black, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(Coord::Type(bitIndex + 1ull + 8ull), to, Piece::create(Black, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(Coord::Type(bitIndex + 1ull + 8ull), to, Piece::create(Black, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 1ull + 8ull), to, board.squares[to]);
        }

        leftCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    auto rightCaptures = pawns & ~Bitboard::rightLine;
    rightCaptures >>= 8ull;
    rightCaptures <<= 1ull;
    auto rightCapturesEp = rightCaptures;
    rightCaptures &= board.bitboards[White];
    while(rightCaptures != Bitboard::null) {
        const auto bitIndex = Bitboard::bitScan(rightCaptures);
        const auto to = Coord::Type(bitIndex);
        
        if (bitIndex < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(Coord::Type(bitIndex - 1ull + 8ull), to, Piece::create(Black, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(Coord::Type(bitIndex - 1ull + 8ull), to, Piece::create(Black, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(Coord::Type(bitIndex - 1ull + 8ull), to, Piece::create(Black, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(Coord::Type(bitIndex - 1ull + 8ull), to, Piece::create(Black, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 1ull + 8ull), to, board.squares[to]);
        }

        rightCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        const auto bitIndex = Board::enpassant(board);
        Bitboard::Type enpassantBitboard = Bitboard::fromIndex(bitIndex);
        const auto to = Coord::Type(bitIndex);

        leftCapturesEp &= enpassantBitboard;
        if (leftCapturesEp != Bitboard::null) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(Coord::Type(bitIndex + 1ull + 8ull), to);
        }

        rightCapturesEp &= enpassantBitboard;
        if (rightCapturesEp != Bitboard::null) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(Coord::Type(bitIndex - 1ull + 8ull), to);
        }
    }
}
}

//@TODO(low): After debugging inline all generator's parts in this function
template <Color::Type COLOR> 
void Generator::forBoard(MoveBuffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    forPawns<COLOR>(buffer, board);
    forKnights<COLOR>(buffer, board);
    forBishops<COLOR>(buffer, board);
    forRooks<COLOR>(buffer, board);
    forKings<COLOR>(buffer, board);
    forQueens<COLOR>(buffer, board);
}

void Generator::forBoard(MoveBuffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forBoard<White>(buffer, board);
    } else {
        forBoard<Black>(buffer, board);
    }
}

// Explicit template instantiations
template void Generator::forKnights<White>(MoveBuffer&, const Board::Type&);
template void Generator::forKnights<Black>(MoveBuffer&, const Board::Type&);

template void Generator::forKings<White>(MoveBuffer&, const Board::Type&);
template void Generator::forKings<Black>(MoveBuffer&, const Board::Type&);

template void Generator::forBishops<White>(MoveBuffer&, const Board::Type&);
template void Generator::forBishops<Black>(MoveBuffer&, const Board::Type&);

template void Generator::forRooks<White>(MoveBuffer&, const Board::Type&);
template void Generator::forRooks<Black>(MoveBuffer&, const Board::Type&);

template void Generator::forQueens<White>(MoveBuffer&, const Board::Type&);
template void Generator::forQueens<Black>(MoveBuffer&, const Board::Type&);

template void Generator::forBoard<White>(MoveBuffer&, const Board::Type&);
template void Generator::forBoard<Black>(MoveBuffer&, const Board::Type&);

void Generator::initTables() {
    Tables::initTables();
    Magic::initTables();
}
