#ifndef BOARDS_HPP
#define BOARDS_HPP

#include <string>

#include "core.hpp"

#include "bitboards.hpp"
#include "pieces.hpp"
#include "coords.hpp"
#include "enpassants.hpp"
#include "castles.hpp"
#include "moves.hpp"
#include "score.hpp"
#include "PST.hpp"
#include "zobrist.hpp"

#include <iostream>

namespace Board {
    struct Info {
        Castle::Type castle = Castle::null;
        Enpassant::Type enpassant = Enpassant::null;

        Move::Buffer buffer;

        Zobrist::Type zobrist = 0;
    };

    struct Type {
        Bitboard::Type bitboards[makeUNumspeed(1) << Piece::usedBits];
        Piece::Type squares[makeUNumspeed(1) << Coord::usedBits];
        
        Color::Type turn = Black;

        UNumspeed halfmoveClock = makeUNumspeed(0);
        UNumspeed fullmoveNumber = makeUNumspeed(1);

        Score::Type positionalScore = Score::Draw;
        Score::Type materialScore   = Score::Draw;

        Zobrist::Type zobrist;

        Info info[MAX_DEPTH+1];
        Info *depthPtr = info;
        UNumspeed depthOffset = 0;
        Info *depthCounter = depthPtr;
    };

    extern Board::Type master;

    inline Numspeed ply(const Type& board) {
        return Numspeed(board.depthPtr - board.depthCounter - board.depthOffset);
    }
        
    inline Move::Buffer &currentBuffer(Type& board) {
        return board.depthPtr->buffer;
    }

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

    inline bool isRepeat(const Type &board) {
        for (auto depth = board.info; depth < board.depthPtr; ++depth) {
            if (depth->zobrist == board.zobrist) {
                std::cout << "FUCK!!!\n";
                return true;
            }
        }

        return false;
    }

    inline void setPiece(Type &board, const Piece::Type piece, const Coord::Type coord) {
        board.bitboards[piece] |= Bitboard::fromCoord(coord);
        board.bitboards[piece & Color::typeMask] |= Bitboard::fromCoord(coord);

        board.positionalScore += PST::tables[piece][coord];
        board.materialScore   += Score::pieceToScoreTable[piece];

        board.zobrist ^= Zobrist::table[piece][coord];

        board.squares[coord] = piece; 
    }

    inline void removePiece(Type &board, const Coord::Type coord) {
        board.bitboards[board.squares[coord]] ^= Bitboard::fromCoord(coord);
        board.bitboards[board.squares[coord] & Color::typeMask] ^= Bitboard::fromCoord(coord);

        board.positionalScore -= PST::tables[board.squares[coord]][coord];
        board.materialScore   -= Score::pieceToScoreTable[board.squares[coord]];

        board.zobrist ^= Zobrist::table[board.squares[coord]][coord];

        board.squares[coord] = Piece::null; 
    }

    template <Piece::Type PIECE>
    inline void setPiece(Type &board, const Coord::Type coord) {
        board.bitboards[PIECE] |= Bitboard::fromCoord(coord);
        board.bitboards[PIECE & Color::typeMask] |= Bitboard::fromCoord(coord);

        board.positionalScore += PST::tables[PIECE][coord];
        board.materialScore   += Score::pieceToScoreTable[PIECE];

        board.zobrist ^= Zobrist::table[PIECE][coord];

        board.squares[coord] = PIECE; 
    }

    template <Piece::Type PIECE>
    inline void removePiece(Type &board, const Coord::Type coord) {
        board.bitboards[PIECE] ^= Bitboard::fromCoord(coord);
        board.bitboards[PIECE & Color::typeMask] ^= Bitboard::fromCoord(coord);

        board.positionalScore -= PST::tables[PIECE][coord];
        board.materialScore   -= Score::pieceToScoreTable[PIECE];

        board.zobrist ^= Zobrist::table[PIECE][coord];

        board.squares[coord] = Piece::null;
    }

    void setFromFen(Type&, const std::string);
    std::string toFen(const Type&);

    std::string show(const Type&);
}

#endif /* BOARDS_HPP */
