#include "checker.hpp"

#include "cstf.hpp"
#include "bitboards.hpp"
#include "generatorTables.hpp"
#include "magics.hpp"

using namespace Checker;

template <Color::Type WHO> 
Boolspeed Checker::isCheck(const Board::Type &board) {
    const auto kingBitboard = board.bitboards[Piece::create(WHO, King)];
    ASSERT(kingBitboard != Bitboard::null);

    const auto kingPosition = Bitboard::bitScan(kingBitboard);

    return isAttacked<WHO>(board, kingPosition);
}

template <Color::Type WHO> 
Boolspeed Checker::isAttacked(const Board::Type &board, const Coord::Type who) {
    const auto OPP = WHO == White? Black: White; // Not Color::inv, because in slow with templates

    auto result = Bitboard::null;

    // Bishop & Queen attacks
    if (Tables::bishopFullMasks[who] & (board.bitboards[Piece::create(OPP, Bishop)] | board.bitboards[Piece::create(OPP, Queen)])) {
        const Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
        UNumspeed magicIndex = Magic::bishopOffsets[who] 
                + UNumspeed(((nonEmpty & Tables::bishopMasks[who]) * Magic::bishopMagics[who]) 
                    >> (Magic::bishopMaskShifts[who]));

        result |= (Magic::bishopData[magicIndex] 
         & (board.bitboards[Piece::create(OPP, Bishop)] | board.bitboards[Piece::create(OPP, Queen)]));
    }

    // Rook & Queen attacks
    if (Tables::rookFullMasks[who] & (board.bitboards[Piece::create(OPP, Rook)] | board.bitboards[Piece::create(OPP, Queen)])) {
        const Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
        UNumspeed magicIndex = Magic::rookOffsets[who] 
            + UNumspeed(((nonEmpty & Tables::rookMasks[who]) * Magic::rookMagics[who]) 
                >> (Magic::rookMaskShifts[who]));

        result |= (Magic::rookData[magicIndex] 
         & (board.bitboards[Piece::create(OPP, Rook)] | board.bitboards[Piece::create(OPP, Queen)]));
    }

    result |=
        // King attacks
        (Tables::kingMasks[who] & board.bitboards[Piece::create(OPP, King)])
      | // Knight attacks
        (board.bitboards[Piece::create(OPP, Knight)] & Tables::knightMasks[who])
      | // Pawn atacks
        (Tables::pawnCaptureMasks[WHO][who] & board.bitboards[Piece::create(OPP, Pawn)]);

    return result;
}

// Explicit template instantiations
template Boolspeed Checker::isCheck<White>(const Board::Type&);
template Boolspeed Checker::isCheck<Black>(const Board::Type&);

template Boolspeed Checker::isAttacked<White>(const Board::Type&, const Coord::Type);
template Boolspeed Checker::isAttacked<Black>(const Board::Type&, const Coord::Type);
