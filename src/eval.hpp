#ifndef EVAL_HPP
#define EVAL_HPP

#include "score.hpp"
#include "boards.hpp"
#include "colors.hpp"
#include "PST.hpp"

namespace Eval {
    template <Color::Type WHO> 
    Score::Type material(const Board::Type& board) {
        const auto OPP = WHO == White? Black: White;
        Score::Type result = 0;

        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Pawn)])   * Score::Pawn;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Knight)]) * Score::Knight;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Bishop)]) * Score::Bishop;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Rook)])   * Score::Rook;
        result += Bitboard::enabledCount(board.bitboards[Piece::create(WHO, Queen)])  * Score::Queen;
    
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Pawn)])   * Score::Pawn;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Knight)]) * Score::Knight;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Bishop)]) * Score::Bishop;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Rook)])   * Score::Rook;
        result -= Bitboard::enabledCount(board.bitboards[Piece::create(OPP, Queen)])  * Score::Queen;

        return result;
    }
    
    template <Color::Type WHO> 
    Score::Type positional(const Board::Type &board) {
        Score::Type result = 0;

        forCoord(x)
        forCoord(y) {
            const auto square = Coord::create(x, y);
            const auto piece = board.squares[square];

            // @TODO: const auto PSTValue = PST::tables[piece][square];

            // @TODO: result += PSTValue;
        }

        if (WHO == White) {
            return result;
        } else {
            return -result;
        }
    }
}

#endif /* EVAL_HPP */
