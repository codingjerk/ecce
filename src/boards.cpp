#include "boards.hpp"

#include <sstream>

#include "cstf.hpp"

using namespace Board;

void setPositionFromFen(Type &board, const std::string fen) {
    UNumspeed cursor = makeUNumspeed(0);
    char current = fen[cursor++];

    forRawYCoords(y) {
        forRawXCoords(x) {
            if (fen[cursor] <= '8') {
                x += UNumspeed(fen[cursor] - '1');
            } else {
                const auto piece = Piece::fromChar(fen[cursor]);
                const auto coord = Coord::fromRaw(x, y);
                setPiece(board, piece, coord);
            }

            ++cursor;
        }

        // Skipping '/' character
        ++cursor;
    }
}

void setTurnFromFen(Type &board, const std::string fen) {
    //@TODO(FAST, USES): Use Color::fromChar
    ASSERT(fen == "w" || fen == "b");

    const auto color = fen == "w"? White: Black;

    setTurn(board, color);
}

void setCastleFromFen(Type &board, const std::string fen) {
    //@TODO(IMPORTANT)
}

void setEnpassantFromFen(Type &board, const std::string fen) {
    if (fen == "-") {
        setEnpassant(board, nullptr);
    } else {
        Coord::Type enpassant = Coord::fromString(fen); 
        setEnpassant(board, &enpassant);
    }
}

void setHalfmoveClockFromFen(Type &board, const UNumspeed value) {
    board.halfmoveClock = value;
}

void setFullmoveNumberFromFen(Type &board, const UNumspeed value) {
    board.fullmoveNumber = value;
}

void Board::setPiece(Type &board, const Piece::Type piece, const Coord::Type coord) {
    board.bitboards[piece] |= Bitboard::fromCoord(coord);
}

void Board::setTurn(Type& board, const Color color) {
    board.turn = color;
}

void Board::setEnpassant(Type& board, const Coord::Type *enpassant) {
    board.enpassant = enpassant;
}

void Board::setFromFen(Type &board, const std::string fen) {
    std::stringstream fenStream(fen);

    std::string positionPart;
    fenStream >> positionPart;
    setPositionFromFen(board, positionPart);

    std::string colorPart;
    fenStream >> colorPart;
    setTurnFromFen(board, colorPart);

    std::string castlePart;
    fenStream >> castlePart;
    setCastleFromFen(board, castlePart);

    std::string enapassantPart;
    fenStream >> enapassantPart;
    setEnpassantFromFen(board, enapassantPart);

    UNumspeed halfmoveClockPart;
    fenStream >> halfmoveClockPart;
    setHalfmoveClockFromFen(board, halfmoveClockPart);

    UNumspeed fullmoveNumberPart;
    fenStream >> fullmoveNumberPart;
    setFullmoveNumberFromFen(board, fullmoveNumberPart);
}

std::string Board::toFen(const Type board) {

}

std::string Board::show(const Type board) {

}
