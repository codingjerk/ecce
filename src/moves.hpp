#ifndef MOVES_HPP
#define MOVES_HPP

#include <string>

#include "core.hpp"

#include "coords.hpp"
#include "pieces.hpp"
#include "boards.hpp"

namespace Move {
    using Type = UNumspeed;
    
    const UNumspeed BUFFER_SIZE = 256;
    using Buffer = Type[BUFFER_SIZE];

    Type fromString(const std::string);

    const UNumspeed enpassantFlag = makeNumspeed(1);
    const UNumspeed castleLongFlag = makeNumspeed(2);
    const UNumspeed castleShortFlag = makeNumspeed(3);
    const UNumspeed pawnDoubleFlag = makeNumspeed(4);
    const UNumspeed promotionFlag = makeNumspeed(5);

    std::string show(const Type);

    const UNumspeed coordsBits = Coord::usedBits * makeUNumspeed(2);
    const UNumspeed coordsOffset = makeUNumspeed(0);
    const UNumspeed coordsMask = (makeUNumspeed(1) << coordsBits) - makeUNumspeed(1);

    const UNumspeed specialBits = makeUNumspeed(4); // for 15 special moves
    const UNumspeed specialOffset = coordsOffset + coordsBits;
    const UNumspeed specialMask = ((makeUNumspeed(1) << specialBits) - makeUNumspeed(1)) << specialOffset;

    const UNumspeed captureBits = Piece::usedBits;
    const UNumspeed captureOffset = specialOffset + specialBits;
    const UNumspeed captureMask = Piece::typeMask << captureOffset;

    const UNumspeed promotionBits = Piece::usedBits;
    const UNumspeed promotionOffset = captureOffset + captureBits;
    const UNumspeed promotionMask = Piece::typeMask << promotionOffset;

    const UNumspeed usedBits = coordsBits + specialBits + captureBits + promotionBits;
    const UNumspeed typeMask = (makeUNumspeed(1) << usedBits) - makeUNumspeed(1);

    inline Type fromString(const std::string text, Board::Type &board) {
        Move::Type simple = fromString(text);

        // @TODO: Create methods Move::from and Move::to
        Coord::Type from = (simple >> Coord::usedBits) & Coord::typeMask; 
        Coord::Type to = simple & Coord::typeMask;

        // Captures
        simple |= (board.squares[to] << captureOffset);

        // PawnDoubles
        if (board.squares[from] & Pawn) {
            if (from - to == 16 || to - from == 16) {
                simple |= (pawnDoubleFlag << specialOffset);
            }//v
            // v
            // Promotions
            else if (text.size() == 5) {
                Color::Type we = (board.squares[from]) & Color::typeMask;
                Piece::Type promoted = Piece::fromChar(text[4]) & Dignity::typeMask | we;
                simple |= (promoted << promotionOffset)
                       |  (promotionFlag << specialOffset);
            }//v
            // v
            // Enpassants
            else if (to == Board::enpassant(board)) {
                simple |= (enpassantFlag << specialOffset);
            }
        }//v
        // v
        // Castles
        else if (board.squares[from] & King && (from == Coord::E1 || from == Coord::E8)) {
            if (to == Coord::C1 || to == Coord::C8) {
                simple |= (castleLongFlag << specialOffset);
            } else if (to == Coord::G1 || to == Coord::G8) {
                simple |= (castleShortFlag << specialOffset);
            }
        }

        return simple;
    }

    inline UNumspeed special(const Type move) {
        return (move & specialMask) >> specialOffset;
    }

    inline Boolspeed isCapture(const Type move) {
        return move & captureMask;
    }

    inline Type create(const Coord::Type from, const Coord::Type to, const Piece::Type captured = Piece::null) {
        return (captured << captureOffset) | (from << Coord::usedBits) | to;
    }

    inline Type promotion(const Coord::Type from, const Coord::Type to, const Piece::Type promoted, const Piece::Type captured = Piece::null) {
        return create(from, to, captured) 
             | (promoted << promotionOffset)
             | (promotionFlag << specialOffset);
    }

    inline Type pawnDouble(const Coord::Type from, const Coord::Type to) {
        return create(from, to) 
             | (pawnDoubleFlag << specialOffset);
    }

    template <Color::Type COLOR>
    inline Type enpassant(const Coord::Type from, const Coord::Type to) {
        const auto OPP = COLOR == White? Black: White; // Color::inv is bad idea with templates

        return create(from, to, Piece::create(OPP, Pawn)) 
             | (enpassantFlag << specialOffset);
    }

    template <Color::Type COLOR> inline Type castleLong();
    
    template <>
    inline Type castleLong<White>() {
        return create(Coord::E1, Coord::C1) 
             | (castleLongFlag << specialOffset);
    }

    template <>
    inline Type castleLong<Black>() {
        return create(Coord::E8, Coord::C8) 
             | (castleLongFlag << specialOffset);
    }

    template <Color::Type COLOR> inline Type castleShort();

    template <>
    inline Type castleShort<White>() {
        return create(Coord::E1, Coord::G1) 
             | (castleShortFlag << specialOffset);
    }

    template <>
    inline Type castleShort<Black>() {
        return create(Coord::E8, Coord::G8) 
             | (castleShortFlag << specialOffset);
    }
}

#endif /* MOVES_HPP */
