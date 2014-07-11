#include "checker.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

using namespace Checker;

template <Color::Type WHO> 
Boolspeed Checker::isCheck(const Board::Type &board) {
    const auto kingBitboard = board.bitboards[Piece::create(WHO, King)];
    const auto kingPosition = Bitboard::bitScan(kingBitboard);

    return isAttacked<WHO>(board, kingPosition);
}

template <Color::Type WHO> 
Boolspeed Checker::isAttacked(const Board::Type &board, const Coord::Type who) {
    //@TODO: Rewrite in piece-centric style
    const Bitboard::Type whoBitboard = Bitboard::fromCoord(who);
    Bitboard::Type attacks = Bitboard::null;

    const auto OPP = Color::inv(WHO);

    auto bitboard = board.bitboards[Piece::create(OPP, Knight)];
    while(bitboard != Bitboard::null) {
        const auto from = Bitboard::bitScan(bitboard);
        bitboard ^= Bitboard::fromIndex(from);

        attacks |= (~board.bitboards[OPP]) & Tables::knightMasks[from];
    }

    Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    bitboard = board.bitboards[Piece::create(OPP, Bishop)] | board.bitboards[Piece::create(OPP, Queen)];
    while(bitboard != Bitboard::null) {
        const auto from = Bitboard::bitScan(bitboard);
        bitboard ^= Bitboard::fromIndex(from);

        const UNumspeed magicIndex = Magic::bishopOffsets[from] 
            + UNumspeed(((nonEmpty & Tables::bishopMasks[from]) * Magic::bishopMagics[from]) 
                >> (Magic::bishopMaskShifts[from]));

        attacks |= Magic::bishopData[magicIndex] & (~board.bitboards[OPP]);
    }

    bitboard = board.bitboards[Piece::create(OPP, Rook)] | board.bitboards[Piece::create(OPP, Queen)];
    while(bitboard != Bitboard::null) {
        const auto from = Bitboard::bitScan(bitboard);
        bitboard ^= Bitboard::fromIndex(from);

        const UNumspeed magicIndex = Magic::rookOffsets[from] 
            + UNumspeed(((nonEmpty & Tables::rookMasks[from]) * Magic::rookMagics[from]) 
                >> (Magic::rookMaskShifts[from]));

        attacks |= Magic::rookData[magicIndex] & (~board.bitboards[OPP]);
    }

    bitboard = Tables::pawnCaptureMasks[WHO][who];
    if (bitboard & board.bitboards[Piece::create(OPP, Pawn)]) return makeBoolspeed(1);

    bitboard = Tables::kingMasks[who];
    if (bitboard & board.bitboards[Piece::create(OPP, King)]) return makeBoolspeed(1);

    return (whoBitboard & attacks);
}

// Explicit template instantiations
template Boolspeed Checker::isCheck<White>(const Board::Type&);
template Boolspeed Checker::isCheck<Black>(const Board::Type&);

template Boolspeed Checker::isAttacked<White>(const Board::Type&, const Coord::Type);
template Boolspeed Checker::isAttacked<Black>(const Board::Type&, const Coord::Type);
