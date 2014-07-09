#include "boards.hpp"

#include <sstream>

#include "cstf.hpp"

using namespace Board;

void Board::setCastle(Type& board, const Castle::Type castle) {
    board.info[board.depth].castle = castle;
}

void Board::setEnpassant(Type& board, const Enpassant::Type enpassant) {
    board.info[board.depth].enpassant = enpassant;
}

void setCastleAll(Type& board, const Castle::Type castle) {
    for (UNumspeed depth = 0; depth <= MAX_DEPTH; ++depth) {
        board.info[depth].castle = castle;
    }
}

void setEnpassantAll(Type& board, const Enpassant::Type enpassant) {
    for (UNumspeed depth = 0; depth <= MAX_DEPTH; ++depth) {
        board.info[depth].enpassant = enpassant;
    }
}

Castle::Type Board::castle(const Type& board) {
    return board.info[board.depth].castle;
}

Enpassant::Type Board::enpassant(const Type& board) {
    return board.info[board.depth].enpassant;
}

void setPositionFromFen(Type &board, const std::string fen) {
    for (auto& bitboard: board.bitboards) {
        bitboard = Bitboard::null;
    }

    for (auto& square: board.squares) {
        square = Piece::null;
    }

    UNumspeed cursor = makeUNumspeed(0);

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

        ++cursor;
    }
}

std::string getFenPosition(const Type &board) {
    std::string result;
    result.reserve(64+8);

    forRawYCoords(y) {
        UNumspeed skipped = makeUNumspeed(0);
        forRawXCoords(x) {
            const auto coord = Coord::fromRaw(x, y);
            const auto piece = board.squares[coord];

            if (piece == Piece::null) {
                ++skipped;
            } else {
                if (skipped != 0) {
                    result += (skipped + '0');
                    skipped = makeUNumspeed(0);
                }

                result += Piece::show(piece);
            }
        }

        if (skipped != 0) result += (skipped + '0');
        if (y != Coord::componentLower) result += "/";
    }

    return result;
}

void Board::setPiece(Type &board, const Piece::Type piece, const Coord::Type coord) {
    ASSERT(board.squares[coord] == Piece::null);

    board.bitboards[piece] |= Bitboard::fromCoord(coord);
    board.bitboards[piece & Color::typeMask] |= Bitboard::fromCoord(coord);
    board.squares[coord] = piece; 
}

void Board::removePiece(Type &board, const Coord::Type coord) {
    ASSERT(board.squares[coord] != Piece::null);

    board.bitboards[board.squares[coord]] ^= Bitboard::fromCoord(coord);
    board.bitboards[board.squares[coord] & Color::typeMask] ^= Bitboard::fromCoord(coord);
    board.squares[coord] = Piece::null; 
}

void Board::setFromFen(Type &board, const std::string fen) {
    std::stringstream fenStream(fen);

    std::string positionPart;
    fenStream >> positionPart;
    setPositionFromFen(board, positionPart);

    std::string turnPart;
    fenStream >> turnPart;
    board.turn = Color::fromString(turnPart);

    std::string castlePart;
    fenStream >> castlePart;
    setCastleAll(board, Castle::fromString(castlePart));

    std::string enapassantPart;
    fenStream >> enapassantPart;
    setEnpassantAll(board, Enpassant::fromString(enapassantPart));

    UNumspeed halfmoveClockPart;
    fenStream >> halfmoveClockPart;
    board.halfmoveClock = halfmoveClockPart;

    UNumspeed fullmoveNumberPart;
    fenStream >> fullmoveNumberPart;
    board.fullmoveNumber = fullmoveNumberPart;
}

std::string Board::toFen(const Type &board) {
    std::stringstream resultStream;

    resultStream << getFenPosition(board) << " ";
    resultStream << Color::show(board.turn) << " ";
    resultStream << Castle::show(Board::castle(board)) << " ";
    resultStream << Enpassant::show(Board::enpassant(board)) << " ";
    resultStream << board.halfmoveClock << " ";
    resultStream << board.fullmoveNumber;

    return resultStream.str();
}

std::string Board::show(const Type&) {
    return "@TODO(IMPORTANT): Write Board::show() function";
}
