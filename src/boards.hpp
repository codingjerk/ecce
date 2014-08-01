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
        Bitboard::Type bitboards[makeUNumspeed(1) << Piece::usedBits];
        Piece::Type squares[makeUNumspeed(1) << Coord::usedBits];
        
        Color::Type turn = Black;

        UNumspeed halfmoveClock = makeUNumspeed(0);
        UNumspeed fullmoveNumber = makeUNumspeed(1);

        Info info[MAX_DEPTH+1];
        Info *depthPtr = info;
    };

    extern Board::Type master;

    inline Castle::Type castle(const Type& board) {
        return board.depthPtr->castle;
    }
    
    inline void castle(Type& board, const Castle::Type castle) {
        board.depthPtr->castle = castle;
    }
    
    inline Enpassant::Type enpassant(const Type& board) {
        return board.depthPtr->enpassant;
    }
    
    inline void enpassant(Type& board, const Enpassant::Type enpassant) {
        board.depthPtr->enpassant = enpassant;
    }

    inline void setPiece(Type &board, const Piece::Type piece, const Coord::Type coord) {
        board.bitboards[piece] |= Bitboard::fromCoord(coord);
        board.bitboards[piece & Color::typeMask] |= Bitboard::fromCoord(coord);
        board.squares[coord] = piece; 
    }

    inline void removePiece(Type &board, const Coord::Type coord) {
        board.bitboards[board.squares[coord]] ^= Bitboard::fromCoord(coord);
        board.bitboards[board.squares[coord] & Color::typeMask] ^= Bitboard::fromCoord(coord);
        board.squares[coord] = Piece::null; 
    }

    void setFromFen(Type&, const std::string);
    std::string toFen(const Type&);

    std::string show(const Type&);
}

#endif /* BOARDS_HPP */
