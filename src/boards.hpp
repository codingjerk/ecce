#ifndef BOARDS_HPP
#define BOARDS_HPP

#include <string>

#include "core.hpp"

#include "bitboards.hpp"
#include "pieces.hpp"
#include "coords.hpp"
#include "enpassants.hpp"
#include "castles.hpp"

namespace Board {
    struct Info {
        Castle::Type castle = Castle::null;
        Enpassant::Type enpassant = Enpassant::null;
    };

    struct Type {
        Bitboard::Type bitboards[makeUNumspeed(1) << Piece::usedBits] = {Bitboard::null};
        Piece::Type squares[makeUNumspeed(1) << Coord::usedBits] = {Piece::null};
        
        Color::Type turn = Black;

        UNumspeed halfmoveClock = makeUNumspeed(0);
        UNumspeed fullmoveNumber = makeUNumspeed(1);

        UNumspeed depth = 0;
        Info info[MAX_DEPTH];
    };

    void setCastle(Type&, const Castle::Type);
    void setEnpassant(Type&, const Enpassant::Type);

    Castle::Type castle(const Type&);
    Enpassant::Type enpassant(const Type&);

    void setPiece(Type&, const Piece::Type, const Coord::Type);
    void removePiece(Type&, const Coord::Type);

    void setFromFen(Type&, const std::string);
    std::string toFen(const Type&);

    std::string show(const Type&);
}

#endif /* BOARDS_HPP */
