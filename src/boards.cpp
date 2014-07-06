#include "boards.hpp"

#include <sstream>

#include "cstf.hpp"

using namespace Board;

void setPositionFromFen(Type &board, const std::string fen) {
    // Cleaning bitboards
    for (auto& bitboard: board.bitboards) {
        bitboard = makeUNum64(0);
    }

    UNumspeed cursor = makeUNumspeed(0);

    forRawYCoords(y) {
        forRawXCoords(x) {
            if (fen[cursor] <= '8') {
                x += UNumspeed(fen[cursor] - '1') * makeUNumspeed(8);
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

std::string getFenPosition(const Type &board) {
    std::string result;
    result.reserve(64+8);

    forRawYCoords(y) {
        UNumspeed skipped = makeUNumspeed(0);
        forRawXCoords(x) {
            const auto coord = Coord::fromRaw(x, y);
            auto piece = getPiece(board, coord);

            if (piece == nullptr) {
                ++skipped;
            } else {
                if (skipped != 0) {
                    result += (skipped + '0');
                    skipped = makeUNumspeed(0);
                }

                result += Piece::show(*piece);
            }
        }

        if (skipped != 0) result += (skipped + '0');
        if (y != Coord::componentLower) result += "/";
    }

    return result;
}

void Board::setPiece(Type &board, const Piece::Type piece, const Coord::Type coord) {
    board.bitboards[piece] |= Bitboard::fromCoord(coord);
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
    board.castle = Castle::fromString(castlePart);

    std::string enapassantPart;
    fenStream >> enapassantPart;
    board.enpassant = Enpassant::fromString(enapassantPart);

    UNumspeed halfmoveClockPart;
    fenStream >> halfmoveClockPart;
    board.halfmoveClock = halfmoveClockPart;

    UNumspeed fullmoveNumberPart;
    fenStream >> fullmoveNumberPart;
    board.fullmoveNumber = fullmoveNumberPart;
}

const Piece::Type *Board::getPiece(const Type &board, const Coord::Type coord) {
    forColors(color) 
    forDignities(dignity) {
        auto piece = new Piece::Type();
        *piece = Piece::create(color, dignity);
        if (board.bitboards[*piece] & Bitboard::fromCoord(coord)) {
            return piece;
        }
    }

    return nullptr;
}

std::string Board::toFen(const Type &board) {
    std::stringstream resultStream;

    resultStream << getFenPosition(board) << " ";
    resultStream << Color::show(board.turn) << " ";
    resultStream << Castle::show(board.castle) << " ";
    resultStream << Enpassant::show(board.enpassant) << " ";
    resultStream << board.halfmoveClock << " ";
    resultStream << board.fullmoveNumber;

    return resultStream.str();
}

std::string Board::show(const Type &board) {
    std::string result;

    forColors(color) 
    forDignities(dignity) {
        auto piece = Piece::create(color, dignity);
        result += Bitboard::show(board.bitboards[piece]) + "\n";
    }

    return result;
}
