#include "mover.hpp"

#include "generatorTables.hpp"
#include "checker.hpp"
#include "castles.hpp"

#include <iostream>

//                         [from coord] [to coord]
Castle::Type castleChanging[makeUNumspeed(1) << Coord::usedBits][makeUNumspeed(1) << Coord::usedBits];

//@TODO(FAST): Make it template
Boolspeed Move::make(Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;
    // @TODO(FAST): rewrite this shit
    if ((board.squares[from] == Piece::create(White, Pawn))
     && (to == from + 16ull)) {// 2 lines up 
        Board::enpassant(board, to - 8ull);
    } else if (board.squares[from] == Piece::create(Black, Pawn)
            && to == from - 16ull) {// 2 lines up 
        Board::enpassant(board, to + 8ull);
    } else {
        Board::enpassant(board, Enpassant::null);
    }

    Board::castle(board, oldCastle & castleChanging[from][to]);
    
    //@TODO(FAST): Rewrite
    if (Move::isEnpassant(move)) {
        Piece::Type captured = (move & Move::captureMask) >> Move::captureOffset;
        if (captured == Piece::create(Black, Pawn)) {
            Board::removePiece(board, to - 8ull);
            Board::setPiece(board, board.squares[from], to);
            Board::removePiece(board, from);
        } else {
            Board::removePiece(board, to + 8ull);
            Board::setPiece(board, board.squares[from], to);
            Board::removePiece(board, from);
        }
    } else {
        if (Move::isCapture(move)) Board::removePiece(board, to);
        if (Move::isPromotion(move)) {
            const auto promoted = (move & Move::promotionMask) >> Move::promotionOffset;
            Board::setPiece(board, promoted, to);
        } else {
            Board::setPiece(board, board.squares[from], to);
        }
        Board::removePiece(board, from);
    }

    Color::invert(board.turn);

    if (Move::isCastleLong(move)) {
        const Coord::Type rookFrom = (board.turn != White)? Coord::fromString("a1"): Coord::fromString("a8");
        const Coord::Type rookTo   = (board.turn != White)? Coord::fromString("d1"): Coord::fromString("d8");

        Board::setPiece(board, board.squares[rookFrom], rookTo);
        Board::removePiece(board, rookFrom);

        if (board.turn != White) {
            if (Checker::isAttacked<White>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, from)) return makeBoolspeed(0);
        } else {
            if (Checker::isAttacked<Black>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, from)) return makeBoolspeed(0);
        }
    } else if (Move::isCastleShort(move)) {
        const Coord::Type rookFrom = (board.turn != White)? Coord::fromString("h1"): Coord::fromString("h8");
        const Coord::Type rookTo   = (board.turn != White)? Coord::fromString("f1"): Coord::fromString("f8");

        Board::setPiece(board, board.squares[rookFrom], rookTo);
        Board::removePiece(board, rookFrom);

        if (board.turn != White) {
            if (Checker::isAttacked<White>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<White>(board, from)) return makeBoolspeed(0);
        } else {
            if (Checker::isAttacked<Black>(board, rookTo)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, to)) return makeBoolspeed(0);
            if (Checker::isAttacked<Black>(board, from)) return makeBoolspeed(0);
        }
    }

    return makeBoolspeed(1);
}

void Move::unmake(Type move, Board::Type& board) {
    Color::invert(board.turn);

    //@TODO: Coord::getFrom
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    //@TODO: Rewrite
    if (Move::isEnpassant(move)) {
        Piece::Type captured = (move & Move::captureMask) >> Move::captureOffset;
        if (captured == Piece::create(Black, Pawn)) {
            Board::setPiece(board, board.squares[to], from);
            Board::removePiece(board, to);
            Board::setPiece(board, captured, to - 8ull);
        } else {
            Board::setPiece(board, board.squares[to], from);
            Board::removePiece(board, to);
            Board::setPiece(board, captured, to + 8ull);
        }
    } else {
        if (Move::isPromotion(move)) {
            const auto promotedColor = ((move & Move::promotionMask) >> Move::promotionOffset) & Color::typeMask;
            Board::setPiece(board, Piece::create(promotedColor, Pawn), from);
        } else {
            Board::setPiece(board, board.squares[to], from);
        }
        Board::removePiece(board, to);
        //@TODO: Move::getCaptured
        if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
    }

    if (Move::isCastleLong(move)) {
        const Coord::Type rookFrom = (board.turn == White)? Coord::fromString("a1"): Coord::fromString("a8");
        const Coord::Type rookTo   = (board.turn == White)? Coord::fromString("d1"): Coord::fromString("d8");

        Board::setPiece(board, board.squares[rookTo], rookFrom);
        Board::removePiece(board, rookTo);
    } else if (Move::isCastleShort(move)) {
        const Coord::Type rookFrom = (board.turn == White)? Coord::fromString("h1"): Coord::fromString("h8");
        const Coord::Type rookTo   = (board.turn == White)? Coord::fromString("f1"): Coord::fromString("f8");

        Board::setPiece(board, board.squares[rookTo], rookFrom);
        Board::removePiece(board, rookTo);
    }

    ++board.depth;
}

void Move::initTables() {
    const Castle::Type castleAll = Castle::fromString("KQkq");
    const Castle::Type castleNone = Castle::fromString("-");

    forCoord(x)
    forCoord(y) {
        const auto from = Coord::create(x, y);
        
        forCoord(x)
        forCoord(y) {
            const auto to = Coord::create(x, y);

            auto castle = castleAll;

            if (from == Coord::fromString("e8") || to == Coord::fromString("e8")) {
                castle &= ~(Castle::blackQueen | Castle::blackKing);
            } 

            if (from == Coord::fromString("e1") || to == Coord::fromString("e1")) {
                castle &= ~(Castle::whiteQueen | Castle::whiteKing);
            } 

            if (from == Coord::fromString("a8") || to == Coord::fromString("a8")) {
                castle &= ~(Castle::blackQueen);
            }

            if (from == Coord::fromString("a1") || to == Coord::fromString("a1")) {
                castle &= ~(Castle::whiteQueen);
            }

            if (from == Coord::fromString("h8") || to == Coord::fromString("h8")) {
                castle &= ~(Castle::blackKing);
            }

            if (from == Coord::fromString("h1") || to == Coord::fromString("h1")) {
                castle &= ~(Castle::whiteKing);
            }

            castleChanging[from][to] = castle;
        }
    }
}
