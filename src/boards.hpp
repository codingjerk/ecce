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

		Zobrist::Type zobrist;
        UNumspeed halfmoveClock;

        Move::Buffer buffer;
        
        Move::Type killer1, killer2;
        UNumspeed killer1Used = 0, killer2Used = 0;

        Coord::Type lastMoved;
    };

    struct Type {
        Bitboard::Type bitboards[makeUNumspeed(1) << Piece::usedBits];
        Piece::Type squares[makeUNumspeed(1) << Coord::usedBits];
        
        Color::Type turn = Black;

        UNumspeed initialFullmoveNumber = makeUNumspeed(1);

        Score::Type positionalScore = Score::Draw;
        Score::Type materialScore   = Score::Draw;

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

	inline void xorzobrist(const Type& board, Zobrist::Type delta) {
		board.depthPtr->zobrist ^= delta;
	}

	inline void copyzobrist(const Type& board) {
		(board.depthPtr + 1)->zobrist = board.depthPtr->zobrist ^ Zobrist::turnKey;
	}

	inline void copyclock(const Type& board) {
        (board.depthPtr + 1)->halfmoveClock = board.depthPtr->halfmoveClock + 1;
	}

	inline void resetclock(const Type& board) {
        (board.depthPtr + 1)->halfmoveClock = 0;
	}

    inline UNumspeed fullMoveNumber(const Type& board) {
        return board.initialFullmoveNumber + (board.depthPtr - board.info) / 2;
    }

	inline bool isRepeat(const Type& board) {
        for (auto depth = board.depthPtr - 4; depth >= (board.depthPtr - board.depthPtr->halfmoveClock); depth -= 2) {
			if (depth->zobrist == board.depthPtr->zobrist) {
				return true;
			}
		}

		return false;
	}

    inline bool isFifty(const Type& board) {
        return (board.depthPtr->halfmoveClock >= 50);
    }

    // @TODO: Send COLOR as template parameter
	template <bool CHANGE_ZOBRIST>
    inline void setPiece(Type &board, const Piece::Type piece, const Coord::Type coord) {
        board.bitboards[piece] |= Bitboard::fromCoord(coord);
        board.bitboards[piece & Color::typeMask] |= Bitboard::fromCoord(coord);

        board.positionalScore += PST::tables[piece][coord];
        board.materialScore   += Score::pieceToScoreTable[piece];

		if (CHANGE_ZOBRIST) xorzobrist(board, Zobrist::table[piece][coord]);

        board.squares[coord] = piece; 
    }

	template <bool CHANGE_ZOBRIST>
    inline void removePiece(Type &board, const Coord::Type coord) {
        board.bitboards[board.squares[coord]] ^= Bitboard::fromCoord(coord);
        board.bitboards[board.squares[coord] & Color::typeMask] ^= Bitboard::fromCoord(coord);

        board.positionalScore -= PST::tables[board.squares[coord]][coord];
		board.materialScore   -= Score::pieceToScoreTable[board.squares[coord]];

		if (CHANGE_ZOBRIST) xorzobrist(board, Zobrist::table[board.squares[coord]][coord]);

        board.squares[coord] = Piece::null; 
    }

	template <Piece::Type PIECE, bool CHANGE_ZOBRIST>
    inline void setPiece(Type &board, const Coord::Type coord) {
        board.bitboards[PIECE] |= Bitboard::fromCoord(coord);
        board.bitboards[PIECE & Color::typeMask] |= Bitboard::fromCoord(coord);

        board.positionalScore += PST::tables[PIECE][coord];
		board.materialScore   += Score::pieceToScoreTable[PIECE];

		if (CHANGE_ZOBRIST) xorzobrist(board, Zobrist::table[PIECE][coord]);

        board.squares[coord] = PIECE; 
    }

	template <Piece::Type PIECE, bool CHANGE_ZOBRIST>
    inline void removePiece(Type &board, const Coord::Type coord) {
        board.bitboards[PIECE] ^= Bitboard::fromCoord(coord);
        board.bitboards[PIECE & Color::typeMask] ^= Bitboard::fromCoord(coord);

        board.positionalScore -= PST::tables[PIECE][coord];
		board.materialScore   -= Score::pieceToScoreTable[PIECE];

		if (CHANGE_ZOBRIST) xorzobrist(board, Zobrist::table[PIECE][coord]);

        board.squares[coord] = Piece::null;
    }

    // Fast versions of {set,remove}Piece
    inline void setPieceFast(Type &board, const Piece::Type piece, const Coord::Type coord) {
        board.bitboards[piece] |= Bitboard::fromCoord(coord);
        board.bitboards[piece & Color::typeMask] |= Bitboard::fromCoord(coord);

        board.squares[coord] = piece; 
    }

    inline void removePieceFast(Type &board, const Coord::Type coord) {
        board.bitboards[board.squares[coord]] ^= Bitboard::fromCoord(coord);
        board.bitboards[board.squares[coord] & Color::typeMask] ^= Bitboard::fromCoord(coord);

        board.squares[coord] = Piece::null; 
    }

	template <Piece::Type PIECE>
    inline void setPieceFast(Type &board, const Coord::Type coord) {
        board.bitboards[PIECE] |= Bitboard::fromCoord(coord);
        board.bitboards[PIECE & Color::typeMask] |= Bitboard::fromCoord(coord);

        board.squares[coord] = PIECE; 
    }

	template <Piece::Type PIECE>
    inline void removePieceFast(Type &board, const Coord::Type coord) {
        board.bitboards[PIECE] ^= Bitboard::fromCoord(coord);
        board.bitboards[PIECE & Color::typeMask] ^= Bitboard::fromCoord(coord);

        board.squares[coord] = Piece::null;
    }

    void setFromFen(Type&, const std::string);
    std::string toFen(const Type&);

    std::string show(const Type&);

    inline void copy(Type& result, const Type& original) {
        setFromFen(result, toFen(original));
    }
}

#endif /* BOARDS_HPP */
