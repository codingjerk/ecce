//@TODO(TRY): Try use precalculated move buffers for knights, kings and pawns
            // knightMovingIndex = ... magic ...
            // additionalBuffer = knightMoves[from][knightMovingIndex];
            // Buffer::add(buffer, additionalBuffer)

#include "generator.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

using namespace Generator;

void addLegals(MoveBuffer &buffer, const Coord::Type from, Bitboard::Type legals) {
    while(legals != 0) {
        const auto bitIndex = Bitboard::bitScan(legals);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, Coord::Type(bitIndex));

        legals ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void forKnight(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[COLOR]) & Tables::knightMasks[from];

    addLegals(buffer, from, legalSquares);
}

template <Color::Type COLOR>
void forKing(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[White]) & Tables::kingMasks[from];

    addLegals(buffer, from, legalSquares);

    const Bitboard::Type legal = ~(board.bitboards[White] | board.bitboards[Black]);

    if (Castle::is<COLOR, King>(board.castle)) {
        if (Tables::castleNeeded[COLOR][King] & legal) {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, Tables::castleTarget[COLOR][King]);
        }
    }

    if (Castle::is<COLOR, Queen>(board.castle)) {
        if (Tables::castleNeeded[COLOR][Queen] & legal) {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, Tables::castleTarget[COLOR][Queen]);
        }
    }
}

template <Color::Type COLOR>
void forBishop(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::bishopOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) * Magic::bishopMagics[from]) 
            >> (Magic::bishopMaskShifts[from]));

    addLegals(buffer, from, Magic::bishopData[magicIndex] & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void forRook(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = Magic::rookOffsets[from] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
            >> (Magic::rookMaskShifts[from]));

    addLegals(buffer, from, Magic::rookData[magicIndex] & (~board.bitboards[COLOR]));
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

    addLegals(buffer, from, (Magic::rookData[rookMagicIndex] | Magic::bishopData[bishopMagicIndex]) & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void Generator::forKnights(MoveBuffer &buffer, const Board::Type &board) {
    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != 0) {
        const auto bitIndex = Bitboard::bitScan(knights);

        forKnight<COLOR>(buffer, board, Coord::Type(bitIndex));

        knights ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR>
void Generator::forKings(MoveBuffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, King)];
    while(bitboard != 0) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forKing<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forBishops(MoveBuffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != 0) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forBishop<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forRooks(MoveBuffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != 0) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forRook<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

template <Color::Type COLOR> 
void Generator::forQueens(MoveBuffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Queen)];
    while(bitboard != 0) {
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
    //@TODO(IMPORTANT): Enpassant and promotions
    const Bitboard::Type legalSquares = ~(board.bitboards[White] | board.bitboards[Black]);
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];
    const auto onestep = (pawns << makeUNum64(8)) & legalSquares;

    UNumspeed bitIndex;
    auto legals = onestep;
    while(legals != 0) {
        bitIndex = Bitboard::bitScan(legals);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 8ull), Coord::Type(bitIndex));

        legals ^= Bitboard::fromIndex(bitIndex);
    }

    auto twosteps = pawns & Tables::pawnStartLine[White];
    twosteps &= onestep >> 8ull;
    twosteps = (twosteps << 16ull) & legalSquares; 
    while(twosteps != 0) {
        bitIndex = Bitboard::bitScan(twosteps);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 16ull), Coord::Type(bitIndex));

        twosteps ^= Bitboard::fromIndex(bitIndex);
    }

    auto leftCaptures = pawns & ~Bitboard::leftLine;
    leftCaptures <<= 8ull;
    leftCaptures >>= 1ull;
    leftCaptures &= board.bitboards[Black];
    while(leftCaptures != 0) {
        bitIndex = Bitboard::bitScan(leftCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 1ull - 8ull), Coord::Type(bitIndex));

        leftCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    auto rightCaptures = pawns & ~Bitboard::rightLine;
    rightCaptures <<= 8ull;
    rightCaptures <<= 1ull;
    rightCaptures &= board.bitboards[Black];
    while(rightCaptures != 0) {
        bitIndex = Bitboard::bitScan(rightCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 1ull + 8ull), Coord::Type(bitIndex));

        rightCaptures ^= Bitboard::fromIndex(bitIndex);
    }
}

template <> 
void forPawns<Black>(MoveBuffer &buffer, const Board::Type &board) {
    //@TODO: Refactoring using tables?
    //@TODO(IMPORTANT): Enpassant and promotions
    const Bitboard::Type legalSquares = ~(board.bitboards[Black] | board.bitboards[White]);
    const auto pawns = board.bitboards[Piece::create(Black, Pawn)];
    const auto onestep = (pawns >> makeUNum64(8)) & legalSquares;

    auto legals = onestep;
    while(legals != 0) {
        const auto bitIndex = Bitboard::bitScan(legals);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 8ull), Coord::Type(bitIndex));

        legals ^= Bitboard::fromIndex(bitIndex);
    }

    const auto startpawns = pawns & Tables::pawnStartLine[Black];
    auto twosteps = startpawns;
    twosteps &= onestep << 8ull;
    twosteps = (twosteps >> 16ull) & legalSquares; 
    while(twosteps != 0) {
        const auto bitIndex = Bitboard::bitScan(twosteps);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 16ull), Coord::Type(bitIndex));

        twosteps ^= Bitboard::fromIndex(bitIndex);
    }

    auto leftCaptures = pawns & ~Bitboard::leftLine;
    leftCaptures >>= 8ull;
    leftCaptures >>= 1ull;
    leftCaptures &= board.bitboards[White];
    while(leftCaptures != 0) {
        const auto bitIndex = Bitboard::bitScan(leftCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 1ull + 8ull), Coord::Type(bitIndex));

        leftCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    auto rightCaptures = pawns & ~Bitboard::rightLine;
    rightCaptures >>= 8ull;
    rightCaptures <<= 1ull;
    rightCaptures &= board.bitboards[White];
    while(rightCaptures != 0) {
        const auto bitIndex = Bitboard::bitScan(rightCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 1ull - 8ull), Coord::Type(bitIndex));

        rightCaptures ^= Bitboard::fromIndex(bitIndex);
    }
}
}

template <Color::Type COLOR> 
void Generator::forBoard(MoveBuffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    forQueens<COLOR>(buffer, board);
    forRooks<COLOR>(buffer, board);
    forBishops<COLOR>(buffer, board);
    forKings<COLOR>(buffer, board);
    forKnights<COLOR>(buffer, board);
    forPawns<COLOR>(buffer, board);
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

void Generator::initTables() {
    Tables::initTables();
    Magic::initTables();
}
