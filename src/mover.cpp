#include "mover.hpp"

#include "generatorTables.hpp"
#include "checker.hpp"
#include "castles.hpp"

#include <iostream>

//                         [from coord] [to coord]
Castle::Type castleChanging[makeUNumspeed(1) << Coord::usedBits][makeUNumspeed(1) << Coord::usedBits];

Boolspeed makeUsual(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Color::invert(board.turn);

    Board::enpassant(board, Enpassant::null);

    //@TODO(?): add a capture flag?
    if (Move::isCapture(move)) Board::removePiece(board, to);
    
    Board::setPiece(board, board.squares[from], to);

    Board::removePiece(board, from);

    return makeBoolspeed(1);
}

Boolspeed makePromotion(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Color::invert(board.turn);

    Board::enpassant(board, Enpassant::null);

    if (Move::isCapture(move)) Board::removePiece(board, to);
    
    const auto promoted = (move & Move::promotionMask) >> Move::promotionOffset;
    Board::setPiece(board, promoted, to);

    Board::removePiece(board, from);

    return makeBoolspeed(1);
}

Boolspeed makePawnDouble(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    
    --board.depth;

    Color::invert(board.turn);

    if (board.turn != White) {// 2 lines up 
        Board::enpassant(board, to - 8ull);
    } else {
        Board::enpassant(board, to + 8ull);
    }

    Board::setPiece(board, board.squares[from], to);

    Board::removePiece(board, from);

    return makeBoolspeed(1);
}

Boolspeed makeEnpassant(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    --board.depth;

    Color::invert(board.turn);

    Board::enpassant(board, Enpassant::null);

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

    return makeBoolspeed(1);
}

Boolspeed makeCastleWhiteLong(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Color::invert(board.turn);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);

    const Coord::Type rookFrom = Coord::fromString("a1");
    const Coord::Type rookTo   = Coord::fromString("d1");

    Board::setPiece(board, board.squares[rookFrom], rookTo);
    Board::removePiece(board, rookFrom);

    if (Checker::isAttacked<White>(board, rookTo)) return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, to))     return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, from))   return makeBoolspeed(0);

    return makeBoolspeed(1);
}

Boolspeed makeCastleWhiteShort(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Color::invert(board.turn);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);

    const Coord::Type rookFrom = Coord::fromString("h1");
    const Coord::Type rookTo   = Coord::fromString("f1");

    Board::setPiece(board, board.squares[rookFrom], rookTo);
    Board::removePiece(board, rookFrom);

    if (Checker::isAttacked<White>(board, rookTo)) return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, to))     return makeBoolspeed(0);
    if (Checker::isAttacked<White>(board, from))   return makeBoolspeed(0);

    return makeBoolspeed(1);
}

Boolspeed makeCastleBlackLong(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Color::invert(board.turn);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);

    const Coord::Type rookFrom = Coord::fromString("a8");
    const Coord::Type rookTo   = Coord::fromString("d8");

    Board::setPiece(board, board.squares[rookFrom], rookTo);
    Board::removePiece(board, rookFrom);

    if (Checker::isAttacked<Black>(board, rookTo)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, to)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, from)) return makeBoolspeed(0);

    return makeBoolspeed(1);
}

Boolspeed makeCastleBlackShort(Move::Type move, Board::Type& board) {
    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;
    const auto oldCastle = Board::castle(board);

    --board.depth;

    Board::castle(board, oldCastle & castleChanging[from][to]);

    Color::invert(board.turn);

    Board::enpassant(board, Enpassant::null);

    Board::setPiece(board, board.squares[from], to);
    Board::removePiece(board, from);

    const Coord::Type rookFrom = Coord::fromString("h8");
    const Coord::Type rookTo   = Coord::fromString("f8");

    Board::setPiece(board, board.squares[rookFrom], rookTo);
    Board::removePiece(board, rookFrom);

    if (Checker::isAttacked<Black>(board, rookTo)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, to)) return makeBoolspeed(0);
    if (Checker::isAttacked<Black>(board, from)) return makeBoolspeed(0);

    return makeBoolspeed(1);
}

//@TODO(fast): More special flags and functions
Boolspeed (*specialMake[8])(Move::Type, Board::Type&) = {
    makeUsual,
    makeEnpassant,
    makeCastleWhiteLong,
    makeCastleWhiteShort,
    makePawnDouble,
    makePromotion,
    makeCastleBlackLong,
    makeCastleBlackShort
};

//@TODO(FAST): Make it template
Boolspeed Move::make(Type move, Board::Type& board) {
    const auto specialIndex = Move::special(move);
    return specialMake[specialIndex](move, board);    
}

void unmakeUsual(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);
    //@TODO: Move::getCaptured
    if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakeCastleWhiteShort(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);

    const Coord::Type rookFrom = Coord::fromString("h1");
    const Coord::Type rookTo   = Coord::fromString("f1");

    Board::setPiece(board, board.squares[rookTo], rookFrom);
    Board::removePiece(board, rookTo);
}

void unmakeCastleWhiteLong(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);

    const Coord::Type rookFrom = Coord::fromString("a1");
    const Coord::Type rookTo   = Coord::fromString("d1");

    Board::setPiece(board, board.squares[rookTo], rookFrom);
    Board::removePiece(board, rookTo);
}

void unmakeCastleBlackShort(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);

    const Coord::Type rookFrom = Coord::fromString("h8");
    const Coord::Type rookTo   = Coord::fromString("f8");

    Board::setPiece(board, board.squares[rookTo], rookFrom);
    Board::removePiece(board, rookTo);
}

void unmakeCastleBlackLong(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);

    const Coord::Type rookFrom = Coord::fromString("a8");
    const Coord::Type rookTo   = Coord::fromString("d8");

    Board::setPiece(board, board.squares[rookTo], rookFrom);
    Board::removePiece(board, rookTo);
}

void unmakePawnDouble(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    Board::setPiece(board, board.squares[to], from);
    Board::removePiece(board, to);
}

void unmakePromotion(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

    const auto promotedColor = ((move & Move::promotionMask) >> Move::promotionOffset) & Color::typeMask;
    Board::setPiece(board, Piece::create(promotedColor, Pawn), from);

    Board::removePiece(board, to);

    if (Move::isCapture(move)) Board::setPiece(board, (move & Move::captureMask) >> Move::captureOffset, to);
}

void unmakeEnpassant(Move::Type move, Board::Type& board) {
    Color::invert(board.turn);

    ++board.depth;

    const Coord::Type from = (move >> Coord::usedBits) & Coord::typeMask;
    const Coord::Type to = move & Coord::typeMask;

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
}

void (*specialUnmake[8])(Move::Type, Board::Type&) = {
    unmakeUsual,
    unmakeEnpassant,
    unmakeCastleWhiteLong,
    unmakeCastleWhiteShort,
    unmakePawnDouble,
    unmakePromotion,
    unmakeCastleBlackLong,
    unmakeCastleBlackShort
};

void Move::unmake(Type move, Board::Type& board) {
    const auto specialIndex = Move::special(move);
    specialUnmake[specialIndex](move, board);
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
