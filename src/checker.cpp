#include "checker.hpp"

#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

using namespace Checker;

template <Color::Type WHO> 
Boolspeed Checker::isCheck(const Board::Type &board) {
    //@TODO: Rewrite in king-centric style
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

    const auto kingBitboard = board.bitboards[Piece::create(WHO, King)];
    const auto kingPos = Bitboard::bitScan(kingBitboard);

    bitboard = Tables::pawnCaptureMasks[WHO][kingPos];
    if (bitboard & board.bitboards[Piece::create(OPP, Pawn)]) return makeBoolspeed(1);

    bitboard = Tables::kingMasks[kingPos];
    if (bitboard & board.bitboards[Piece::create(OPP, King)]) return makeBoolspeed(1);

    return (board.bitboards[Piece::create(WHO, King)] & attacks);
}

// Explicit template instantiations
template Boolspeed Checker::isCheck<White>(const Board::Type&);
template Boolspeed Checker::isCheck<Black>(const Board::Type&);
