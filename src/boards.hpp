#ifndef BOARDS_HPP
#define BOARDS_HPP

#include <string>

#include "core.hpp"

#include "bitboards.hpp"
#include "pieces.hpp"
#include "coords.hpp"

namespace Board {
    struct Type {
        Bitboard::Type bitboards[makeUNumspeed(1) << Piece::usedBits] = {makeUNum64(0)};
        
        // @TODO(USES): Move to own file to use Color::Type
        Color turn = Black;

        // @TODO(IMPORTANT): Castle flags

        // @TODO(USES): Enpassant type (as simple coord clone wthich can be null), 'cas enapssant may be null
        const Coord::Type *enpassant = nullptr;

        UNumspeed halfmoveClock = makeUNumspeed(0);
        UNumspeed fullmoveNumber = makeUNumspeed(1);

    };

    void setPiece(Type&, const Piece::Type, const Coord::Type);
    void setTurn(Type&, const Color);
    void setEnpassant(Type&, const Coord::Type*);

    void setFromFen(Type&, const std::string);
    std::string toFen(const Type);

    std::string show(const Type);
}

#endif /* BOARDS_HPP */
