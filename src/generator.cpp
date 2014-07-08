//@TODO(GLOBAL): split all functions to capture/silent versions
//@TODO(GLOBAL): add flag capture to capture moves
//@TODO(GLOBAL): add generation functions for bishop, rook and queen
//@TODO(GLOBAL): add function forBoard(const Board::Type&)

#include "generator.hpp"

#include "bitboards.hpp"

using namespace Generator;

Move::Type knightMoveTable[makeUNumspeed(1) << Coord::usedBits];
Move::Type kingMoveTable[makeUNumspeed(1) << Coord::usedBits];

Bitboard::Type pawnStartLine[makeUNumspeed(1) << Color::usedBitsReal];

Bitboard::Type leftLine;
Bitboard::Type rightLine;

//@TODO(FAST, USES): Refactor to arrays
Bitboard::Type whiteKingCastleNeeded;
Coord::Type whiteKingCastleTarged;

Bitboard::Type whiteQueenCastleNeeded;
Coord::Type whiteQueenCastleTarged;

Bitboard::Type blackKingCastleNeeded;
Coord::Type blackKingCastleTarged;

Bitboard::Type blackQueenCastleNeeded;
Coord::Type blackQueenCastleTarged;

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
    const Bitboard::Type legalSquares = (~board.bitboards[COLOR]) & knightMoveTable[from];

    addLegals(buffer, from, legalSquares);
}

template <Color::Type COLOR>
void forKing(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from);

template<> void forKing<White>(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[White]) & kingMoveTable[from];

    addLegals(buffer, from, legalSquares);

    //@TODO(FAST, USES): Refactoring
    if (board.castle | Castle::whiteKing) {
        const Bitboard::Type legal = ~(board.bitboards[White] | board.bitboards[Black]);

        if (whiteKingCastleNeeded & legal) {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, whiteKingCastleTarged);
        }
    }
    if (board.castle | Castle::whiteQueen) {
        const Bitboard::Type legal = ~(board.bitboards[White] | board.bitboards[Black]);

        if (whiteQueenCastleNeeded & legal) {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, whiteQueenCastleTarged);
        }
    }
}

template<> void forKing<Black>(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[Black]) & kingMoveTable[from];

    addLegals(buffer, from, legalSquares);

    //@TODO(FAST, USES): Refactoring
    if (board.castle | Castle::blackKing) {
        const Bitboard::Type legal = ~(board.bitboards[White] | board.bitboards[Black]);

        if (blackKingCastleNeeded & legal) {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, blackKingCastleTarged);
        }
    }
    if (board.castle | Castle::blackQueen) {
        const Bitboard::Type legal = ~(board.bitboards[White] | board.bitboards[Black]);

        if (blackQueenCastleNeeded & legal) {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(from, whiteQueenCastleTarged);
        }
    }
}

void Generator::forKnights(MoveBuffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forKnights<White>(buffer, board);
    } else {
        forKnights<Black>(buffer, board);
    }
}

//@TODO(IMPORTANT): Make it fully templates, based on Color and Dignity
template <Color::Type COLOR>
void Generator::forKnights(MoveBuffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != 0) {
        const auto bitIndex = Bitboard::bitScan(knights);

        forKnight<COLOR>(buffer, board, Coord::Type(bitIndex));

        knights ^= Bitboard::fromIndex(bitIndex);
    }
}

void Generator::forKings(MoveBuffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forKings<White>(buffer, board);
    } else {
        forKings<Black>(buffer, board);
    }
}

template <Color::Type COLOR>
void Generator::forKings(MoveBuffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    auto bitboard = board.bitboards[Piece::create(COLOR, King)];
    while(bitboard != 0) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forKing<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

// Temporary solution, because gcc is stupid dick
namespace Generator {
// Time: 20.386
template <> 
void forPawns<White>(MoveBuffer &buffer, const Board::Type &board) {
    //@TODO(low): Refactoring using tables?
    //@TODO(IMPORTANT): Enpassant and promotions
    buffer[0] = 0;
    const Bitboard::Type legalSquares = ~(board.bitboards[White] | board.bitboards[Black]);
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];
    const auto onestep = (pawns << makeUNum64(8)) & legalSquares;

    auto legals = onestep;
    while(legals != 0) {
        const auto bitIndex = Bitboard::bitScan(legals);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 8ull), Coord::Type(bitIndex));

        legals ^= Bitboard::fromIndex(bitIndex);
    }

    auto twosteps = pawns & pawnStartLine[White];
    twosteps &= onestep >> 8ull;
    twosteps = (twosteps << 16ull) & legalSquares; 
    while(twosteps != 0) {
        const auto bitIndex = Bitboard::bitScan(twosteps);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 16ull), Coord::Type(bitIndex));

        twosteps ^= Bitboard::fromIndex(bitIndex);
    }

    auto leftCaptures = pawns & ~leftLine;
    leftCaptures <<= 8ull;
    leftCaptures >>= 1ull;
    leftCaptures &= board.bitboards[Black];
    while(leftCaptures != 0) {
        const auto bitIndex = Bitboard::bitScan(leftCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 1ull - 8ull), Coord::Type(bitIndex));

        leftCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    auto rightCaptures = pawns & ~rightLine;
    rightCaptures <<= 8ull;
    rightCaptures <<= 1ull;
    rightCaptures &= board.bitboards[Black];
    while(rightCaptures != 0) {
        const auto bitIndex = Bitboard::bitScan(rightCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 1ull + 8ull), Coord::Type(bitIndex));

        rightCaptures ^= Bitboard::fromIndex(bitIndex);
    }
}

template <> 
void forPawns<Black>(MoveBuffer &buffer, const Board::Type &board) {
    //@TODO: Refactoring using tables?
    //@TODO(IMPORTANT): Enpassant and promotions
    buffer[0] = 0;
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

    const auto startpawns = pawns & pawnStartLine[Black];
    auto twosteps = startpawns;
    twosteps &= onestep << 8ull;
    twosteps = (twosteps >> 16ull) & legalSquares; 
    while(twosteps != 0) {
        const auto bitIndex = Bitboard::bitScan(twosteps);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 16ull), Coord::Type(bitIndex));

        twosteps ^= Bitboard::fromIndex(bitIndex);
    }

    auto leftCaptures = pawns & ~leftLine;
    leftCaptures >>= 8ull;
    leftCaptures >>= 1ull;
    leftCaptures &= board.bitboards[White];
    while(leftCaptures != 0) {
        const auto bitIndex = Bitboard::bitScan(leftCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 1ull + 8ull), Coord::Type(bitIndex));

        leftCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    auto rightCaptures = pawns & ~rightLine;
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

void Generator::forPawns(MoveBuffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forPawns<White>(buffer, board);
    } else {
        forPawns<Black>(buffer, board);
    }
}

// Explicit template instantiations
template void Generator::forKnights<White>(MoveBuffer&, const Board::Type&);
template void Generator::forKnights<Black>(MoveBuffer&, const Board::Type&);

template void Generator::forKings<White>(MoveBuffer&, const Board::Type&);
template void Generator::forKings<Black>(MoveBuffer&, const Board::Type&);

void Generator::initTables() {
    //@TODO(low level): Refactoring?
    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;
        if ((x+2ull <= 7ull) && (y+1ull <= 7ull)) toBits |= Bitboard::fromCoord(Coord::create(x+2, y+1));
        if ((x+2ull <= 7ull) && (y-1ull) <= 7ull) toBits |= Bitboard::fromCoord(Coord::create(x+2, y-1));
        if ((x+1ull <= 7ull) && (y+2ull) <= 7ull) toBits |= Bitboard::fromCoord(Coord::create(x+1, y+2));
        if ((x+1ull <= 7ull) && (y-2ull) <= 7ull) toBits |= Bitboard::fromCoord(Coord::create(x+1, y-2));
        if ((x-1ull <= 7ull) && (y+2ull) <= 7ull) toBits |= Bitboard::fromCoord(Coord::create(x-1, y+2));
        if ((x-1ull <= 7ull) && (y-2ull) <= 7ull) toBits |= Bitboard::fromCoord(Coord::create(x-1, y-2));
        if ((x-2ull <= 7ull) && (y+1ull) <= 7ull) toBits |= Bitboard::fromCoord(Coord::create(x-2, y+1));
        if ((x-2ull <= 7ull) && (y-1ull) <= 7ull) toBits |= Bitboard::fromCoord(Coord::create(x-2, y-1));

        knightMoveTable[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;
        if (x+1 <= 7 && y+1 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x+1, y+1));
        if (x+1 <= 7 && y+0 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x+1, y+0));
        if (x+1 <= 7 && y-1 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x+1, y-1));
        if (x+0 <= 7 && y+1 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x+0, y+1));
        if (x+0 <= 7 && y-1 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x+0, y-1));
        if (x-1 <= 7 && y+1 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x-1, y+1));
        if (x-1 <= 7 && y+0 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x-1, y+0));
        if (x-1 <= 7 && y-1 <= 7) toBits |= Bitboard::fromCoord(Coord::create(x-1, y-1));

        kingMoveTable[from] = toBits;
    }

    whiteKingCastleNeeded = Bitboard::fromCoord(Coord::fromString("f1"))
                          | Bitboard::fromCoord(Coord::fromString("g1"));
    whiteKingCastleTarged = Coord::fromString("g1");

    whiteQueenCastleNeeded = Bitboard::fromCoord(Coord::fromString("d1"))
                           | Bitboard::fromCoord(Coord::fromString("c1"))
                           | Bitboard::fromCoord(Coord::fromString("b1"));
    whiteQueenCastleTarged = Coord::fromString("c1");

    blackKingCastleNeeded = Bitboard::fromCoord(Coord::fromString("f8"))
                          | Bitboard::fromCoord(Coord::fromString("g8"));
    blackKingCastleTarged = Coord::fromString("g8");

    blackQueenCastleNeeded = Bitboard::fromCoord(Coord::fromString("d8"))
                           | Bitboard::fromCoord(Coord::fromString("c8"))
                           | Bitboard::fromCoord(Coord::fromString("b8"));
    blackQueenCastleTarged = Coord::fromString("c8");

    pawnStartLine[White] = Bitboard::fromCoord(Coord::fromString("a2"))
                         | Bitboard::fromCoord(Coord::fromString("b2"))
                         | Bitboard::fromCoord(Coord::fromString("c2"))
                         | Bitboard::fromCoord(Coord::fromString("d2"))
                         | Bitboard::fromCoord(Coord::fromString("e2"))
                         | Bitboard::fromCoord(Coord::fromString("f2"))
                         | Bitboard::fromCoord(Coord::fromString("g2"))
                         | Bitboard::fromCoord(Coord::fromString("h2"));

    pawnStartLine[Black] = Bitboard::fromCoord(Coord::fromString("a7"))
                         | Bitboard::fromCoord(Coord::fromString("b7"))
                         | Bitboard::fromCoord(Coord::fromString("c7"))
                         | Bitboard::fromCoord(Coord::fromString("d7"))
                         | Bitboard::fromCoord(Coord::fromString("e7"))
                         | Bitboard::fromCoord(Coord::fromString("f7"))
                         | Bitboard::fromCoord(Coord::fromString("g7"))
                         | Bitboard::fromCoord(Coord::fromString("h7"));

    leftLine = Bitboard::fromCoord(Coord::fromString("a1"))
             | Bitboard::fromCoord(Coord::fromString("a2"))
             | Bitboard::fromCoord(Coord::fromString("a3"))
             | Bitboard::fromCoord(Coord::fromString("a4"))
             | Bitboard::fromCoord(Coord::fromString("a5"))
             | Bitboard::fromCoord(Coord::fromString("a6"))
             | Bitboard::fromCoord(Coord::fromString("a7"))
             | Bitboard::fromCoord(Coord::fromString("a8"));

    rightLine = Bitboard::fromCoord(Coord::fromString("h1"))
              | Bitboard::fromCoord(Coord::fromString("h2"))
              | Bitboard::fromCoord(Coord::fromString("h3"))
              | Bitboard::fromCoord(Coord::fromString("h4"))
              | Bitboard::fromCoord(Coord::fromString("h5"))
              | Bitboard::fromCoord(Coord::fromString("h6"))
              | Bitboard::fromCoord(Coord::fromString("h7"))
              | Bitboard::fromCoord(Coord::fromString("h8"));
}
