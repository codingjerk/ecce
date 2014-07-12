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
    //@TODO: Add statistic miner
    const auto OPP = Color::inv(WHO);

    // Knight attacks
    if (board.bitboards[Piece::create(OPP, Knight)] & Tables::knightMasks[who])
        return makeBoolspeed(1);
    
    // Bishop & Queen attacks
    //@TODO: Try to use simple fuzzy check before magic
    const Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    UNumspeed magicIndex = Magic::bishopOffsets[who] 
            + UNumspeed(((nonEmpty & Tables::bishopMasks[who]) * Magic::bishopMagics[who]) 
                >> (Magic::bishopMaskShifts[who]));

    if (Magic::bishopData[magicIndex] 
     & (board.bitboards[Piece::create(OPP, Bishop)] | board.bitboards[Piece::create(OPP, Queen)]))
        return makeBoolspeed(2);

    // Rook & Queen attacks
    magicIndex = Magic::rookOffsets[who] 
        + UNumspeed(((nonEmpty & Tables::rookMasks[who]) * Magic::rookMagics[who]) 
            >> (Magic::rookMaskShifts[who]));

    if (Magic::rookData[magicIndex] 
     & (board.bitboards[Piece::create(OPP, Rook)] | board.bitboards[Piece::create(OPP, Queen)]))
        return makeBoolspeed(3);

    if (Tables::pawnCaptureMasks[WHO][who] & board.bitboards[Piece::create(OPP, Pawn)]) 
        return makeBoolspeed(4);

    if (Tables::kingMasks[who] & board.bitboards[Piece::create(OPP, King)]) 
        return makeBoolspeed(5);

    return makeBoolspeed(0);
}

// Explicit template instantiations
template Boolspeed Checker::isCheck<White>(const Board::Type&);
template Boolspeed Checker::isCheck<Black>(const Board::Type&);

template Boolspeed Checker::isAttacked<White>(const Board::Type&, const Coord::Type);
template Boolspeed Checker::isAttacked<Black>(const Board::Type&, const Coord::Type);
