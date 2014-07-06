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

void setTurnFromFen(Type &board, const std::string fen) {
    ASSERT(fen == "w" || fen == "b");

    const auto color = Color::fromString(fen);

    setTurn(board, color);
}

void setCastleFromFen(Type &board, const std::string fen) {
    //@TODO(IMPORTANT)
}

void setEnpassantFromFen(Type &board, const std::string fen) {
    if (fen == "-") {
        setEnpassant(board, nullptr);
    } else {
        auto enpassant = new Coord::Type();
        *enpassant = Coord::fromString(fen); 
        setEnpassant(board, enpassant);
    }
}

void setHalfmoveClockFromFen(Type &board, const UNumspeed value) {
    board.halfmoveClock = value;
}

void setFullmoveNumberFromFen(Type &board, const UNumspeed value) {
    board.fullmoveNumber = value;
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

void Board::setTurn(Type& board, const Color::Type color) {
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

const Piece::Type *Board::getPiece(const Type &board, const Coord::Type coord) {
    //@TODO(USES): Write macroses forColors(color) and forDignities(dignity)
    for (Color::Type color = Black; color <= White; color += White) {
        for (Dignity dignity = Pawn; dignity <= Queen; ++dignity) {
            auto piece = new Piece::Type();
            *piece = Piece::create(color, dignity);
            if (board.bitboards[*piece] & Bitboard::fromCoord(coord)) {
                return piece;
            }
        }
    }

    return nullptr;
}

std::string Board::toFen(const Type &board) {
    std::stringstream resultStream;

    resultStream << getFenPosition(board) << " ";
    resultStream << Color::show(board.turn) << " ";
    //@TODO(IMPORTANT)
    resultStream << "KQkq ";
    //@TODO(USES): Use Enpassant::show(enpassant)
    resultStream << ((board.enpassant == nullptr)? "- ": (Coord::show(*board.enpassant) + " "));
    resultStream << board.halfmoveClock << " ";
    resultStream << board.fullmoveNumber;

    return resultStream.str();
}

std::string Board::show(const Type &board) {
    std::string result;

    for (Color::Type color = Black; color <= White; color += White) {
        for (Dignity dignity = Pawn; dignity <= Queen; ++dignity) {
            auto piece = Piece::create(color, dignity);
            result += Bitboard::show(board.bitboards[piece]) + "\n";
        }
    }

    return result;
}
