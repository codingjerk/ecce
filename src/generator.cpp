#include "generator.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

inline void addLegals(Move::Buffer &buffer, const Board::Type &board, const Coord::Type &from, Bitboard::Type legals) {
    while(legals != Bitboard::null) {
        const auto to = Bitboard::bitScan(legals);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to, board.squares[to]);

        legals ^= Bitboard::fromCoord(to);
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
void Generator::knights(Move::Buffer &buffer, const Board::Type &board) {
    auto knights = board.bitboards[Piece::create(COLOR, Knight)];
    while(knights != Bitboard::null) {
        const auto to = Bitboard::bitScan(knights);

        forKnight<COLOR>(buffer, board, to);

        knights ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR>
void Generator::kings(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, King)];

    forKing<COLOR>(buffer, board, Coord::Type(Bitboard::bitScan(bitboard)));
}

template <Color::Type COLOR> 
void Generator::bishops(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Bishop)];
    while(bitboard != Bitboard::null) {
        const auto to = Bitboard::bitScan(bitboard);

        forBishop<COLOR>(buffer, board, to);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR> 
void Generator::rooks(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Rook)];
    while(bitboard != Bitboard::null) {
        const auto to = Bitboard::bitScan(bitboard);

        forRook<COLOR>(buffer, board, to);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

template <Color::Type COLOR> 
void Generator::queens(Move::Buffer &buffer, const Board::Type &board) {
    auto bitboard = board.bitboards[Piece::create(COLOR, Queen)];
    while(bitboard != Bitboard::null) {
        const auto to = Bitboard::bitScan(bitboard);

        forQueen<COLOR>(buffer, board, to);

        bitboard ^= Bitboard::fromCoord(to);
    }
}

// Temporary solution, because gcc is stupid dick
namespace Generator {
template <> 
void pawns<White>(Move::Buffer &buffer, const Board::Type &board) {
    const Bitboard::Type legalSquares = ~(board.bitboards[White] | board.bitboards[Black]);
    const auto pawns = board.bitboards[Piece::create(White, Pawn)];
    const auto onestep = (pawns << makeUNum64(8)) & legalSquares;

    Coord::Type to;
    auto workingBB = onestep;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        if (to >= 56ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(to - 8ull, to, Piece::create(White, Knight));
            buffer[buffer[0]-1] = Move::promotion(to - 8ull, to, Piece::create(White, Bishop));
            buffer[buffer[0]-2] = Move::promotion(to - 8ull, to, Piece::create(White, Rook));
            buffer[buffer[0]-3] = Move::promotion(to - 8ull, to, Piece::create(White, Queen));
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(to - 8ull, to);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = ((pawns & Tables::pawnStartLine[White] & (onestep >> 8ull)) << 16ull) & legalSquares;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(to - 16ull, to);

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = (pawns & ~Bitboard::leftLine) << 7ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[Black];
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        if (to >= 56ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(to - 7ull, to, Piece::create(White, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(to - 7ull, to, Piece::create(White, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(to - 7ull, to, Piece::create(White, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(to - 7ull, to, Piece::create(White, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(to - 7ull, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = (pawns & ~Bitboard::rightLine) << 9ull;
    auto rightCapturesEp = workingBB;
    workingBB &= board.bitboards[Black];
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        if (to >= 56ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(to - 9ull, to, Piece::create(White, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(to - 9ull, to, Piece::create(White, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(to - 9ull, to, Piece::create(White, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(to - 9ull, to, Piece::create(White, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(to - 9ull, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        to = Board::enpassant(board);
        workingBB = Bitboard::fromCoord(to);

        leftCapturesEp &= workingBB;
        if (leftCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(to - 7ull, to);
        }

        rightCapturesEp &= workingBB;
        if (rightCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<White>(to - 9ull, to);
        }
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

        if (to < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(to + 8ull, to, Piece::create(Black, Knight));
            buffer[buffer[0]-1] = Move::promotion(to + 8ull, to, Piece::create(Black, Bishop));
            buffer[buffer[0]-2] = Move::promotion(to + 8ull, to, Piece::create(Black, Rook));
            buffer[buffer[0]-3] = Move::promotion(to + 8ull, to, Piece::create(Black, Queen));
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(to + 8ull, to);
        }
        
        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = ((pawns & Tables::pawnStartLine[Black] & (onestep << 8ull)) >> 16ull) & legalSquares;
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        ++buffer[0];
        buffer[buffer[0]] = Move::pawnDouble(to + 16ull, to);

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = (pawns & ~Bitboard::leftLine) >> 9ull;
    auto leftCapturesEp = workingBB;
    workingBB &= board.bitboards[White];
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);

        if (to < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(to + 9ull, to, Piece::create(Black, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(to + 9ull, to, Piece::create(Black, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(to + 9ull, to, Piece::create(Black, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(to + 9ull, to, Piece::create(Black, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(to + 9ull, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    workingBB = (pawns & ~Bitboard::rightLine) >> 7ull;
    auto rightCapturesEp = workingBB;
    workingBB &= board.bitboards[White];
    while(workingBB != Bitboard::null) {
        to = Bitboard::bitScan(workingBB);
        
        if (to < 8ull) { //Pawn goes at last line
            buffer[0] += 4;
            buffer[buffer[0]] = Move::promotion(to + 7ull, to, Piece::create(Black, Knight), board.squares[to]);
            buffer[buffer[0]-1] = Move::promotion(to + 7ull, to, Piece::create(Black, Bishop), board.squares[to]);
            buffer[buffer[0]-2] = Move::promotion(to + 7ull, to, Piece::create(Black, Rook), board.squares[to]);
            buffer[buffer[0]-3] = Move::promotion(to + 7ull, to, Piece::create(Black, Queen), board.squares[to]);
        } else {
            ++buffer[0];
            buffer[buffer[0]] = Move::create(to + 7ull, to, board.squares[to]);
        }

        workingBB ^= Bitboard::fromCoord(to);
    }

    if (Board::enpassant(board) != Enpassant::null) {
        to = Board::enpassant(board);
        workingBB = Bitboard::fromIndex(to);

        leftCapturesEp &= workingBB;
        if (leftCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(to + 9ull, to);
        }

        rightCapturesEp &= workingBB;
        if (rightCapturesEp) {
            ++buffer[0];
            buffer[buffer[0]] = Move::enpassant<Black>(to + 7ull, to);
        }
    }
}
}

template <Color::Type COLOR> 
void Generator::phase(Move::Buffer &buffer, const Board::Type &board) {
    buffer[0] = 0;

    pawns<COLOR>(buffer, board);
    knights<COLOR>(buffer, board);
    bishops<COLOR>(buffer, board);
    rooks<COLOR>(buffer, board);
    kings<COLOR>(buffer, board);
    queens<COLOR>(buffer, board);
}

// Explicit template instantiations
template void Generator::phase<White>(Move::Buffer&, const Board::Type&);
template void Generator::phase<Black>(Move::Buffer&, const Board::Type&);

template void Generator::pawns<White>(Move::Buffer&, const Board::Type&);
template void Generator::pawns<Black>(Move::Buffer&, const Board::Type&);

template void Generator::knights<White>(Move::Buffer&, const Board::Type&);
template void Generator::knights<Black>(Move::Buffer&, const Board::Type&);

template void Generator::bishops<White>(Move::Buffer&, const Board::Type&);
template void Generator::bishops<Black>(Move::Buffer&, const Board::Type&);

template void Generator::rooks<White>(Move::Buffer&, const Board::Type&);
template void Generator::rooks<Black>(Move::Buffer&, const Board::Type&);

template void Generator::kings<White>(Move::Buffer&, const Board::Type&);
template void Generator::kings<Black>(Move::Buffer&, const Board::Type&);

template void Generator::queens<White>(Move::Buffer&, const Board::Type&);
template void Generator::queens<Black>(Move::Buffer&, const Board::Type&);
