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

Bitboard::Type knightMoveTable[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type kingMoveTable[makeUNumspeed(1) << Coord::usedBits];
Bitboard::Type bishopMoveTable[makeUNumspeed(1) << Coord::usedBits];

Bitboard::Type pawnStartLine[makeUNumspeed(1) << Color::usedBitsReal];

Bitboard::Type leftLine;
Bitboard::Type rightLine;

Bitboard::Type upLine;
Bitboard::Type downLine;

// Magics @TODO: Move to own file
UNumspeed m_bOffset[64];

Bitboard::Type *bishopData;

const int m_bBits[64] =
{
    6,  5,  5,  5,  5,  5,  5,  6,
    5,  5,  5,  5,  5,  5,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  5,  5,  5,  5,  5,  5,
    6,  5,  5,  5,  5,  5,  5,  6
};

/*const Bitboard::Type m_bMask[64] =
{
    0x0040201008040200ull, 0x0020100804020000ull, 0x0050080402000000ull, 0x0028440200000000ull,
    0x0014224000000000ull, 0x000a102040000000ull, 0x0004081020400000ull, 0x0002040810204000ull,
    0x0000402010080400ull, 0x0000201008040200ull, 0x0000500804020000ull, 0x0000284402000000ull,
    0x0000142240000000ull, 0x00000a1020400000ull, 0x0000040810204000ull, 0x0000020408102000ull,
    0x0040004020100800ull, 0x0020002010080400ull, 0x0050005008040200ull, 0x0028002844020000ull,
    0x0014001422400000ull, 0x000a000a10204000ull, 0x0004000408102000ull, 0x0002000204081000ull,
    0x0020400040201000ull, 0x0010200020100800ull, 0x0008500050080400ull, 0x0044280028440200ull,
    0x0022140014224000ull, 0x00100a000a102000ull, 0x0008040004081000ull, 0x0004020002040800ull,
    0x0010204000402000ull, 0x0008102000201000ull, 0x0004085000500800ull, 0x0002442800284400ull,
    0x0040221400142200ull, 0x0020100a000a1000ull, 0x0010080400040800ull, 0x0008040200020400ull,
    0x0008102040004000ull, 0x0004081020002000ull, 0x0002040850005000ull, 0x0000024428002800ull,
    0x0000402214001400ull, 0x004020100a000a00ull, 0x0020100804000400ull, 0x0010080402000200ull,
    0x0004081020400000ull, 0x0002040810200000ull, 0x0000020408500000ull, 0x0000000244280000ull,
    0x0000004022140000ull, 0x00004020100a0000ull, 0x0040201008040000ull, 0x0020100804020000ull,
    0x0002040810204000ull, 0x0000020408102000ull, 0x0000000204085000ull, 0x0000000002442800ull,
    0x0000000040221400ull, 0x0000004020100a00ull, 0x0000402010080400ull, 0x0040201008040200ull
};*/

const Bitboard::Type m_bMask[64] =
{
    0x0040201008040200ull, 0x0020100804020000ull, 0x0050080402000000ull, 0x0028440200000000ull, 0x0014224000000000ull, 0x000a102040000000ull, 0x0004081020400000ull, 0x0002040810204000ull,
    0x0000402010080400ull, 0x0000201008040200ull, 0x0000500804020000ull, 0x0000284402000000ull, 0x0000142240000000ull, 0x00000a1020400000ull, 0x0000040810204000ull, 0x0000020408102000ull,
    0x0040004020100800ull, 0x0020002010080400ull, 0x0050005008040200ull, 0x0028002844020000ull, 0x0014001422400000ull, 0x000a000a10204000ull, 0x0004000408102000ull, 0x0002000204081000ull,
    0x0020400040201000ull, 0x0010200020100800ull, 0x0008500050080400ull, 0x0044280028440200ull, 0x0022140014224000ull, 0x00100a000a102000ull, 0x0008040004081000ull, 0x0004020002040800ull,
    0x0010204000402000ull, 0x0008102000201000ull, 0x0004085000500800ull, 0x0002442800284400ull, 0x0040221400142200ull, 0x0020100a000a1000ull, 0x0010080400040800ull, 0x0008040200020400ull,
    0x0008102040004000ull, 0x0004081020002000ull, 0x0002040850005000ull, 0x0000024428002800ull, 0x0000402214001400ull, 0x004020100a000a00ull, 0x0020100804000400ull, 0x0010080402000200ull,
    0x0004081020400000ull, 0x0002040810200000ull, 0x0000020408500000ull, 0x0000000244280000ull, 0x0000004022140000ull, 0x00004020100a0000ull, 0x0040201008040000ull, 0x0020100804020000ull,
    0x0002040810204000ull, 0x0000020408102000ull, 0x0000000204085000ull, 0x0000000002442800ull, 0x0000000040221400ull, 0x0000004020100a00ull, 0x0000402010080400ull, 0x0040201008040200ull
};

UNum64 bishopMagics[makeUNumspeed(1) << Coord::usedBits] = {
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

Bitboard::Type m_bMult[64] =
{
    0x0040080100420440ull, 0x0000201401060400ull, 0x0000802004900180ull, 0x0200000022024400ull,
    0x0000000400411091ull, 0x1008002100411000ull, 0x0002104202100200ull, 0x0000240100d01000ull,
    0x0004810802208000ull, 0x00a0021002008000ull, 0x0200040c08020200ull, 0x0000448405040000ull,
    0x5000000442020002ull, 0x0020402203100000ull, 0x0000888401600000ull, 0x4000410808400080ull,
    0x2084008400400100ull, 0x0182040104000200ull, 0x2003200080800100ull, 0x0000200208809400ull,
    0x0000024200802800ull, 0x1001008040410400ull, 0x0008440220000801ull, 0x0201282004001000ull,
    0x0004040028004100ull, 0x0001010200142200ull, 0x0020080040008240ull, 0x0001020a00040050ull,
    0x0100020080180480ull, 0x0008109000080044ull, 0x2002021080200100ull, 0x2010043000041000ull,
    0x0001020401008080ull, 0x0042040000410800ull, 0x0800420001009200ull, 0x01108c0000802000ull,
    0x0004014024010002ull, 0x0001221010008200ull, 0x0005100005040800ull, 0x0008048020202200ull,
    0x0001000080829000ull, 0x0002000141042010ull, 0x8000800048200801ull, 0x0001000890400800ull,
    0x0018040082004000ull, 0x000c200204040008ull, 0x0020000418028100ull, 0x0020001404042800ull,
    0x00002200c1041000ull, 0x0000040084242100ull, 0x0024009004200020ull, 0x4001040421008000ull,
    0x0009042408800000ull, 0x0080080094208000ull, 0x0010204400808100ull, 0x000004100a120400ull,
    0x0001050800840400ull, 0x0000440220102004ull, 0x0082080208000011ull, 0x0482021080004000ull,
    0x8008048100010040ull, 0x0090208081000040ull, 0x3002080104008000ull, 0x0410101000802040ull
};

UNum64 bishopShifts[makeUNumspeed(1) << Coord::usedBits] = {
    58, 59, 59, 59, 59, 59, 59, 58, 
    59, 59, 59, 59, 59, 59, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59, 
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59, 
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 59, 59, 59, 59, 59, 59, 
    58, 59, 59, 59, 59, 59, 59, 58
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

#include <iostream>

template <Color::Type COLOR>
void forBishop(MoveBuffer &buffer, const Board::Type &board, const Coord::Type from) {
    int index = m_bOffset[from];
    Bitboard::Type occ = (board.bitboards[Black] | board.bitboards[White]);
    index += int(((occ & m_bMask[63-from]) * m_bMult[63-from]) >> (64 - m_bBits[from]));

    std::cout << Bitboard::show(occ) << "\n";
    std::cout << Bitboard::show(occ & m_bMask[63-from]) << "\n";
    std::cout << Bitboard::show(bishopData[index]) << "\n";

    addLegals(buffer, from, bishopData[index] & (~board.bitboards[COLOR]));
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

#include <iostream>

UNumspeed PopLSB(Bitboard::Type& bb)
{
    UNumspeed f = Bitboard::bitScan(bb);
    bb ^= Bitboard::fromIndex(f);
    return f;
}

Bitboard::Type EnumBits(Bitboard::Type mask, Bitboard::Type n)
{
    Bitboard::Type x = 0;
    while (mask != 0 && n != 0)
    {
        int f = PopLSB(mask);
        int digit = int(n & 1);
        n >>= 1;
        x |= digit * Bitboard::fromIndex(f);
    }
    return x;
}

Bitboard::Type UpRight(const Bitboard::Type& b) { return (b & ~(rightLine|upLine)) << 9; }
Bitboard::Type UpLeft(const Bitboard::Type& b) { return (b & ~(leftLine|upLine)) << 7; }
Bitboard::Type DownRight(const Bitboard::Type& b) { return (b & ~(rightLine|downLine)) >> 7; }
Bitboard::Type DownLeft(const Bitboard::Type& b) { return (b & ~(leftLine|downLine)) >> 9; }

#define TRACE(Shift)                \
    x = Shift(Bitboard::fromIndex(f)); \
    while (x)                       \
    {                               \
        att |= x;                   \
        if (x & occ) break;         \
        x = Shift(x);               \
    }

Bitboard::Type BishopAttacksTrace(int f, const Bitboard::Type& occ)
{
    Bitboard::Type att = 0;
    Bitboard::Type x = 0;
    TRACE(UpRight);
    TRACE(UpLeft);
    TRACE(DownLeft);
    TRACE(DownRight);
    return att;
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

        knightMoveTable[from] = toBits;
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

        kingMoveTable[from] = toBits;
    }

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        auto toBits = Bitboard::null;

        for (Numspeed xDirection = -1; xDirection <= 1; xDirection += 2)
        for (Numspeed yDirection = -1; yDirection <= 1; yDirection += 2) {
            for (Numspeed delta = 1; delta <= 7ull; ++delta) {
                if (UNumspeed(x + delta*xDirection) <= 7ull && UNumspeed(y + delta*yDirection) <= 7ull) {
                    auto to = Coord::create(x + delta*xDirection, y + delta*yDirection);
                    toBits |= Bitboard::fromCoord(to);
                }
            }
        }

        bishopMoveTable[from] = toBits;
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

    // Magics        
    int offset = 0;
    for (int f = 0; f < 64; ++f)
    {
        m_bOffset[f] = offset;
        offset += (1 << m_bBits[f]);
    }
    bishopData = new Bitboard::Type[offset];

    forCoord(x)
    forCoord(y) {
        auto const f = Coord::create(x, y);

        Bitboard::Type mask = m_bMask[63-f];
        int bits = m_bBits[f];
        for (int n = 0; n < (1 << bits); ++n)
        {
            Bitboard::Type occ = EnumBits(mask, n);
            Bitboard::Type att = BishopAttacksTrace(f, occ);
            if (f == Coord::fromString("d3")) {
                std::cout << "---d3---\n";
                std::cout << Bitboard::show(mask) << "\n";
                std::cout << Bitboard::show(occ) << "\n";
                std::cout << Bitboard::show(att) << "\n";
            }
            int index = m_bOffset[f];
            index += int((occ * m_bMult[63-f]) >> (64-bits));
            bishopData[index] = att;
        }
    }
}
