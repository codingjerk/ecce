#include "generator.hpp"

#include "bitboards.hpp"

using namespace Generator;

Move::Type knightMoveTable[makeUNumspeed(1) << Coord::usedBits];

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

    auto bitboard = board.bitboards[Piece::create(COLOR, Knight)];
    while(bitboard != 0) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forKnight<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

// Explicit template instantiations
template void Generator::forKnights<White>(MoveBuffer&, const Board::Type&);
template void Generator::forKnights<Black>(MoveBuffer&, const Board::Type&);

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
}
