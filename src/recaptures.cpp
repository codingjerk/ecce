#include "recaptures.hpp"

#include "generatorTables.hpp"
#include "magics.hpp"

inline void addLegalRecaptures(Move::Buffer &buffer, const Board::Type &board, const Coord::Type to, Bitboard::Type froms) {
    while (froms != Bitboard::null) {
        const auto from = Bitboard::bitScan(froms);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to, board.squares[to]);

        froms ^= Bitboard::fromCoord(from);
    }
}

template <Color::Type COLOR>
inline void addLegalPawnRecaptures(Move::Buffer &buffer, const Board::Type &board, const Coord::Type to, Bitboard::Type froms) {
    while (froms != Bitboard::null) {
        const auto from = Bitboard::bitScan(froms);

        ++buffer[0];
        if (Bitboard::fromCoord(from) & Tables::pawnSubPromotionLine[COLOR]) {
            buffer[buffer[0]] = Move::promotion(from, to, Piece::create(COLOR, Queen), board.squares[to]);
        } else {
            buffer[buffer[0]] = Move::create(from, to, board.squares[to]);
        }

        froms ^= Bitboard::fromCoord(from);
    }
}

inline void addOneLegalRecapture(Move::Buffer &buffer, const Board::Type &board, const Coord::Type to, Bitboard::Type froms) {
    if (froms != Bitboard::null) {
        const auto from = Bitboard::bitScan(froms);

        ++buffer[0];
        buffer[buffer[0]] = Move::create(from, to, board.squares[to]);
    }
}

template <Color::Type COLOR>
void kingsRecaptures(Move::Buffer& buffer, const Board::Type& board, Coord::Type lastMoved) {
    const auto attackers = Tables::kingMasks[lastMoved] & board.bitboards[Piece::create(COLOR, King)];

    addOneLegalRecapture(buffer, board, lastMoved, attackers);
}

template <Color::Type COLOR>
void pawnsRecaptures(Move::Buffer& buffer, const Board::Type& board, Coord::Type lastMoved) {
    MAKEOPP(COLOR);

    const auto attackers = Tables::pawnCaptureMasks[OPP][lastMoved] & board.bitboards[Piece::create(COLOR, Pawn)];

    addLegalPawnRecaptures<COLOR>(buffer, board, lastMoved, attackers);
}

template <Color::Type COLOR>
void knightsRecaptures(Move::Buffer& buffer, const Board::Type& board, Coord::Type lastMoved) {
    const auto attackers = board.bitboards[Piece::create(COLOR, Knight)] & Tables::knightMasks[lastMoved];

    addLegalRecaptures(buffer, board, lastMoved, attackers);
}

template <Color::Type COLOR>
void bishopsRecaptures(Move::Buffer& buffer, const Board::Type& board, Coord::Type lastMoved) {
    const Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    UNumspeed magicIndex = Magic::bishopOffsets[lastMoved]
        + UNumspeed(((nonEmpty & Tables::bishopMasks[lastMoved]) * Magic::bishopMagics[lastMoved])
        >> (Magic::bishopMaskShifts[lastMoved]));

    const auto attackers = (Magic::bishopData[magicIndex] & board.bitboards[Piece::create(COLOR, Bishop)]);

    addLegalRecaptures(buffer, board, lastMoved, attackers);
}

template <Color::Type COLOR>
void rooksRecaptures(Move::Buffer& buffer, const Board::Type& board, Coord::Type lastMoved) {
    const Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    UNumspeed magicIndex = Magic::rookOffsets[lastMoved]
        + UNumspeed(((nonEmpty & Tables::rookMasks[lastMoved]) * Magic::rookMagics[lastMoved])
        >> (Magic::rookMaskShifts[lastMoved]));

    const auto attackers = (Magic::rookData[magicIndex] & board.bitboards[Piece::create(COLOR, Rook)]);

    addLegalRecaptures(buffer, board, lastMoved, attackers);
}

template <Color::Type COLOR>
void queensRecaptures(Move::Buffer& buffer, const Board::Type& board, Coord::Type lastMoved) {
    const Bitboard::Type nonEmpty = (board.bitboards[Black] | board.bitboards[White]);
    UNumspeed magicIndex = Magic::rookOffsets[lastMoved]
        + UNumspeed(((nonEmpty & Tables::rookMasks[lastMoved]) * Magic::rookMagics[lastMoved])
        >> (Magic::rookMaskShifts[lastMoved]));

    auto attackers = (Magic::rookData[magicIndex] & board.bitboards[Piece::create(COLOR, Queen)]);

    magicIndex = Magic::bishopOffsets[lastMoved]
        + UNumspeed(((nonEmpty & Tables::bishopMasks[lastMoved]) * Magic::bishopMagics[lastMoved])
        >> (Magic::bishopMaskShifts[lastMoved]));

    attackers |= (Magic::bishopData[magicIndex] & board.bitboards[Piece::create(COLOR, Queen)]);

    addLegalRecaptures(buffer, board, lastMoved, attackers);
}

template <Color::Type COLOR> 
void Recaptures::phase(Move::Buffer& buffer, const Board::Type& board) {
    buffer[0] = 0;

    const auto lastMoved = board.depthPtr->lastMoved;

    kingsRecaptures<COLOR>(buffer, board, lastMoved);
    pawnsRecaptures<COLOR>(buffer, board, lastMoved);
    knightsRecaptures<COLOR>(buffer, board, lastMoved);
    bishopsRecaptures<COLOR>(buffer, board, lastMoved);
    rooksRecaptures<COLOR>(buffer, board, lastMoved);
    queensRecaptures<COLOR>(buffer, board, lastMoved);
}

template void Recaptures::phase<White>(Move::Buffer&, const Board::Type&);
template void Recaptures::phase<Black>(Move::Buffer&, const Board::Type&);
