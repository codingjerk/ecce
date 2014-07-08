//@TODO(GLOBAL): split all functions to capture/silent versions
//@TODO(GLOBAL): add flag capture to capture moves
//@TODO(GLOBAL): add generation functions for bishop, rook and queen
//@TODO(GLOBAL): add function forBoard(const Board::Type&)
//@TODO(TRY): Try use precalculated move buffers for knights, kings and pawns
            // knightMovingIndex = ... magic ...
            // additionalBuffer = knightMoves[from][knightMovingIndex];
            // Buffer::add(buffer, additionalBuffer)
//@TODO: Global: move all tables to own file

#include "generator.hpp"

#include "bitboards.hpp"

using namespace Generator;

// --- Rooks
UNumspeed rookOffsets[64];
Bitboard::Type *rookData = nullptr;

const UNum64 rookMagics[64] = {
    0x00800011400080a6, 0x004000100120004e, 0x0080100008600082, 0x0080080016500080, 
    0x0080040008000280, 0x0080020005040080, 0x0080108046000100, 0x0080010000204080, 
    0x0010800424400082, 0x00004002c8201000, 0x000c802000100080, 0x00810010002100b8, 
    0x00ca808014000800, 0x0002002884900200, 0x0042002148041200, 0x00010000c200a100, 
    0x00008580004002a0, 0x0020004001403008, 0x0000820020411600, 0x0002120021401a00, 
    0x0024808044010800, 0x0022008100040080, 0x00004400094a8810, 0x0000020002814c21, 
    0x0011400280082080, 0x004a050e002080c0, 0x00101103002002c0, 0x0025020900201000, 
    0x0001001100042800, 0x0002008080022400, 0x000830440021081a, 0x0080004200010084, 
    0x00008000c9002104, 0x0090400081002900, 0x0080220082004010, 0x0001100101000820, 
    0x0000080011001500, 0x0010020080800400, 0x0034010224009048, 0x0002208412000841, 
    0x000040008020800c, 0x001000c460094000, 0x0020006101330040, 0x0000a30010010028, 
    0x0004080004008080, 0x0024000201004040, 0x0000300802440041, 0x00120400c08a0011, 
    0x0080006085004100, 0x0028600040100040, 0x00a0082110018080, 0x0010184200221200, 
    0x0040080005001100, 0x0004200440104801, 0x0080800900220080, 0x000a01140081c200, 
    0x0080044180110021, 0x0008804001001225, 0x00a00c4020010011, 0x00001000a0050009, 
    0x0011001800021025, 0x00c9000400620811, 0x0032009001080224, 0x001400810044086a
};

const UNumspeed rookMaskBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const UNumspeed rookMaskShifts[64] = {
    64 - 12, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 12,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 11, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 10, 64 - 11,
    64 - 12, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 11, 64 - 12
};

// --- Rooks

// @TODO: Move to tables.cpp, hpp
Bitboard::Type knightMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type kingMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type bishopMasks[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type rookMasks[makeUNumspeed(1) << Coord::usedBits];

// @TODO: Move to bitboard.cpp, hpp
Bitboard::Type pawnStartLine[makeUNumspeed(1) << Color::usedBitsReal];

Bitboard::Type leftLine;
Bitboard::Type rightLine;

Bitboard::Type upLine;
Bitboard::Type downLine;

// Magics @TODO: Move to own file
UNumspeed bishopOffsets[64];

Bitboard::Type *bishopData = nullptr;

const UNumspeed bishopMaskBits[64] = {
    6,  5,  5,  5,  5,  5,  5,  6,
    5,  5,  5,  5,  5,  5,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  5,  5,  5,  5,  5,  5,
    6,  5,  5,  5,  5,  5,  5,  6
};

const UNumspeed bishopMaskShifts[64] = {
    64 - 6,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 6,
    64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 7,  64 - 7,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 9,  64 - 9,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 9,  64 - 9,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 7,  64 - 7,  64 - 7,  64 - 7,  64 - 5,  64 - 5,
    64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,
    64 - 6,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 5,  64 - 6
};

const UNum64 bishopMagics[makeUNumspeed(1) << Coord::usedBits] = {
    0x0048610528020080, 0x00c4100212410004, 0x0004180181002010, 0x0004040188108502, 
    0x0012021008003040, 0x0002900420228000, 0x0080808410c00100, 0x000600410c500622, 
    0x00c0056084140184, 0x0080608816830050, 0x00a010050200b0c0, 0x0000510400800181, 
    0x0000431040064009, 0x0000008820890a06, 0x0050028488184008, 0x00214a0104068200, 
    0x004090100c080081, 0x000a002014012604, 0x0020402409002200, 0x008400c240128100, 
    0x0001000820084200, 0x0024c02201101144, 0x002401008088a800, 0x0003001045009000, 
    0x0084200040981549, 0x0001188120080100, 0x0048050048044300, 0x0008080000820012, 
    0x0001001181004003, 0x0090038000445000, 0x0010820800a21000, 0x0044010108210110, 
    0x0090241008204e30, 0x000c04204004c305, 0x0080804303300400, 0x00a0020080080080, 
    0x0000408020220200, 0x0000c08200010100, 0x0010008102022104, 0x0008148118008140, 
    0x0008080414809028, 0x0005031010004318, 0x0000603048001008, 0x0008012018000100, 
    0x0000202028802901, 0x004011004b049180, 0x0022240b42081400, 0x00c4840c00400020, 
    0x0084009219204000, 0x000080c802104000, 0x0002602201100282, 0x0002040821880020, 
    0x0002014008320080, 0x0002082078208004, 0x0009094800840082, 0x0020080200b1a010, 
    0x0003440407051000, 0x000000220e100440, 0x00480220a4041204, 0x00c1800011084800, 
    0x000008021020a200, 0x0000414128092100, 0x0000042002024200, 0x0002081204004200
};

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
    const Bitboard::Type legalSquares = (~board.bitboards[COLOR]) & knightMasks[from];

    addLegals(buffer, from, legalSquares);
}

template <Color::Type COLOR>
void forKing(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from);

template<> void forKing<White>(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    const Bitboard::Type legalSquares = (~board.bitboards[White]) & kingMasks[from];

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
    const Bitboard::Type legalSquares = (~board.bitboards[Black]) & kingMasks[from];

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

template <Color::Type COLOR>
void forBishop(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = bishopOffsets[from] + UNumspeed(((nonEmpty & bishopMasks[from]) * bishopMagics[from]) >> (bishopMaskShifts[from]));

    addLegals(buffer, from, bishopData[magicIndex] & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void forRook(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed magicIndex = rookOffsets[from] + UNumspeed(((nonEmpty & rookMasks[from]) * rookMagics[from]) >> (rookMaskShifts[from]));

    addLegals(buffer, from, rookData[magicIndex] & (~board.bitboards[COLOR]));
}

template <Color::Type COLOR>
void forQueen(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    const UNumspeed rookMagicIndex = rookOffsets[from] + UNumspeed(((nonEmpty & rookMasks[from]) * rookMagics[from]) >> (rookMaskShifts[from]));
    const UNumspeed bishopMagicIndex = bishopOffsets[from] + UNumspeed(((nonEmpty & bishopMasks[from]) * bishopMagics[from]) >> (bishopMaskShifts[from]));

    addLegals(buffer, from, (rookData[rookMagicIndex] | bishopData[bishopMagicIndex]) & (~board.bitboards[COLOR]));
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

void Generator::forBishops(MoveBuffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forBishops<White>(buffer, board);
    } else {
        forBishops<Black>(buffer, board);
    }
}

template <Color::Type COLOR> 
void Generator::forBishops(MoveBuffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != 0) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forBishop<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

void Generator::forRooks(MoveBuffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forRooks<White>(buffer, board);
    } else {
        forRooks<Black>(buffer, board);
    }
}

template <Color::Type COLOR> 
void Generator::forRooks(MoveBuffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != 0) {
        const auto bitIndex = Bitboard::bitScan(bitboard);

        forRook<COLOR>(buffer, board, Coord::Type(bitIndex));

        bitboard ^= Bitboard::fromIndex(bitIndex);
    }
}

void Generator::forQueens(MoveBuffer &buffer, const Board::Type &board) {
    if (board.turn == White) {
        forQueens<White>(buffer, board);
    } else {
        forQueens<Black>(buffer, board);
    }
}

template <Color::Type COLOR> 
void Generator::forQueens(MoveBuffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

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
    buffer[0] = 0;
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

    auto twosteps = pawns & pawnStartLine[White];
    twosteps &= onestep >> 8ull;
    twosteps = (twosteps << 16ull) & legalSquares; 
    while(twosteps != 0) {
        bitIndex = Bitboard::bitScan(twosteps);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex - 16ull), Coord::Type(bitIndex));

        twosteps ^= Bitboard::fromIndex(bitIndex);
    }

    auto leftCaptures = pawns & ~leftLine;
    leftCaptures <<= 8ull;
    leftCaptures >>= 1ull;
    leftCaptures &= board.bitboards[Black];
    while(leftCaptures != 0) {
        bitIndex = Bitboard::bitScan(leftCaptures);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::create(Coord::Type(bitIndex + 1ull - 8ull), Coord::Type(bitIndex));

        leftCaptures ^= Bitboard::fromIndex(bitIndex);
    }

    auto rightCaptures = pawns & ~rightLine;
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

// @TODO: Move to bitboards.cpp, hpp
Bitboard::Type turnBits(Bitboard::Type mask, UNumspeed bitsToTurn) {
    Bitboard::Type result = 0;

    while (mask != 0 && bitsToTurn != 0) {
        UNumspeed first = Bitboard::bitScan(mask);
        mask ^= Bitboard::fromIndex(first);

        UNumspeed digit = UNumspeed(bitsToTurn & 1);
        bitsToTurn >>= 1;
        result |= digit * Bitboard::fromIndex(first);
    }

    return result;
}

// @TODO: Move to bitboard.cpp, hpp
Bitboard::Type up(const Bitboard::Type b) { return (b & ~(upLine)) << 8; }
Bitboard::Type down(const Bitboard::Type b) { return (b & ~(downLine)) >> 8; }
Bitboard::Type left(const Bitboard::Type b) { return (b & ~(leftLine)) >> 1; }
Bitboard::Type right(const Bitboard::Type b) { return (b & ~(rightLine)) << 1; }

Bitboard::Type upRight(const Bitboard::Type b) { return (b & ~(rightLine|upLine)) << 9; }
Bitboard::Type upLeft(const Bitboard::Type b) { return (b & ~(leftLine|upLine)) << 7; }
Bitboard::Type downRight(const Bitboard::Type b) { return (b & ~(rightLine|downLine)) >> 7; }
Bitboard::Type downLeft(const Bitboard::Type b) { return (b & ~(leftLine|downLine)) >> 9; }

Bitboard::Type calculateBishopAttacks(UNumspeed from, const Bitboard::Type nonEmpty)
{
    Bitboard::Type result = 0;
    Bitboard::Type x = 0;

    x = upRight(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = upRight(x);               
    }

    x = upLeft(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = upLeft(x);               
    }

    x = downRight(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = downRight(x);               
    }

    x = downLeft(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = downLeft(x);               
    }

    return result;
}

Bitboard::Type calculateRookAttacks(UNumspeed from, const Bitboard::Type nonEmpty)
{
    Bitboard::Type result = 0;
    Bitboard::Type x = 0;

    x = up(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = up(x);               
    }

    x = down(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = down(x);               
    }

    x = right(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = right(x);               
    }

    x = left(Bitboard::fromIndex(from)); 
    while (x)                       
    {                               
        result |= x;                   
        if (x & nonEmpty) break;         
        x = left(x);               
    }

    return result;
}

void Generator::initTables() {
    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);

        auto toBits = Bitboard::null;
        for (Numspeed xDelta = -2; xDelta <= 2; ++xDelta)
        for (Numspeed yDelta = -2; yDelta <= 2; ++yDelta) {
            if (xDelta == 0 || yDelta == 0 || std::abs(xDelta) == std::abs(yDelta)) continue;

            if ((x + xDelta <= 7ull) && (y + yDelta <= 7ull)) {
                toBits |= Bitboard::fromCoord(Coord::create(x + xDelta, y + yDelta));
            }
        }

        knightMasks[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xDelta = -1; xDelta <= 1; ++xDelta)
        for (Numspeed yDelta = -1; yDelta <= 1; ++yDelta) {
            if (xDelta == 0 && yDelta == 0) continue;

            if ((x + xDelta <= 7ull) && (y + yDelta <= 7ull)) {
                toBits |= Bitboard::fromCoord(Coord::create(x + xDelta, y + yDelta));
            }
        }

        kingMasks[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xDirection = -1; xDirection <= 1; xDirection += 2)
        for (Numspeed yDirection = -1; yDirection <= 1; yDirection += 2) {
            for (Numspeed delta = makeNumspeed(1); delta <= makeNumspeed(6); ++delta) {
                if (UNumspeed(x + delta*xDirection) <= makeUNumspeed(6) && UNumspeed(y + delta*yDirection) <= makeUNumspeed(6)
                 && UNumspeed(x + delta*xDirection) >= makeUNumspeed(1) && UNumspeed(y + delta*yDirection) >= makeUNumspeed(1)) {
                    auto to = Coord::create(x + delta*xDirection, y + delta*yDirection);
                    toBits |= Bitboard::fromCoord(to);
                }
            }
        }

        bishopMasks[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xTo = 1; xTo <= 6; ++xTo) {
            const auto to = Coord::create(xTo, y);

            toBits |= Bitboard::fromCoord(to);
        }

        for (Numspeed yTo = 1; yTo <= 6; ++yTo) {
            const auto to = Coord::create(x, yTo);

            toBits |= Bitboard::fromCoord(to);
        }

        toBits &= ~Bitboard::fromCoord(from);

        rookMasks[from] = toBits;
    }

    //@TODO: Hardcode this definitions
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

    upLine = Bitboard::fromCoord(Coord::fromString("a8"))
           | Bitboard::fromCoord(Coord::fromString("b8"))
           | Bitboard::fromCoord(Coord::fromString("c8"))
           | Bitboard::fromCoord(Coord::fromString("d8"))
           | Bitboard::fromCoord(Coord::fromString("e8"))
           | Bitboard::fromCoord(Coord::fromString("f8"))
           | Bitboard::fromCoord(Coord::fromString("g8"))
           | Bitboard::fromCoord(Coord::fromString("h8"));


    downLine = Bitboard::fromCoord(Coord::fromString("a1"))
             | Bitboard::fromCoord(Coord::fromString("b1"))
             | Bitboard::fromCoord(Coord::fromString("c1"))
             | Bitboard::fromCoord(Coord::fromString("d1"))
             | Bitboard::fromCoord(Coord::fromString("e1"))
             | Bitboard::fromCoord(Coord::fromString("f1"))
             | Bitboard::fromCoord(Coord::fromString("g1"))
             | Bitboard::fromCoord(Coord::fromString("h1"));

    // Magics for bishop, @TODO: Move to magic.cpp, hpp
    UNumspeed offset = makeUNumspeed(0);
    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);
        bishopOffsets[from] = offset;
        offset += (1 << bishopMaskBits[from]);
    }
    bishopData = new Bitboard::Type[offset];

    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);

        Bitboard::Type mask = bishopMasks[from];
        UNumspeed bits = bishopMaskBits[from];
        for (UNumspeed bitsToTurn = makeUNumspeed(0); bitsToTurn < (makeUNumspeed(1) << bits); ++bitsToTurn) {
            Bitboard::Type nonEmpty = turnBits(mask, bitsToTurn);
            Bitboard::Type data = calculateBishopAttacks(from, nonEmpty);
            UNumspeed index = bishopOffsets[from] + UNumspeed((nonEmpty * bishopMagics[from]) >> (makeUNumspeed(64)-bits));
            bishopData[index] = data;
        }
    }

    offset = makeUNumspeed(0);
    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);
        rookOffsets[from] = offset;
        offset += (1 << rookMaskBits[from]);
    }
    rookData = new Bitboard::Type[offset];

    forCoord(x)
    forCoord(y) {
        auto const from = Coord::create(x, y);

        Bitboard::Type mask = rookMasks[from];
        UNumspeed bits = rookMaskBits[from];
        for (UNumspeed bitsToTurn = makeUNumspeed(0); bitsToTurn < (makeUNumspeed(1) << bits); ++bitsToTurn) {
            Bitboard::Type nonEmpty = turnBits(mask, bitsToTurn);
            Bitboard::Type data = calculateRookAttacks(from, nonEmpty);
            UNumspeed index = rookOffsets[from] + UNumspeed((nonEmpty * rookMagics[from]) >> (makeUNumspeed(64) - bits));
            rookData[index] = data;
        }
    }
}
