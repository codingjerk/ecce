#include "boards.hpp"

#include <sstream>
#include <algorithm>

#include "cstf.hpp"

using namespace Board;

Board::Type Board::master;

void setPositionFromFen(Type &board, const std::string fen) {
    for (auto& bitboard: board.bitboards) bitboard = Bitboard::null;
    for (auto& square: board.squares) square = Piece::null;

    for (auto& info: board.info) {
        auto maxKiller = std::max(1ull, std::max(info.killer1Used, info.killer2Used));
        info.killer1Used /= maxKiller;
        info.killer2Used /= maxKiller;
    }

    board.depthPtr = board.info;
    board.materialScore     = 0;
    board.positionalScore   = 0;
    board.depthOffset       = 0;
	board.depthPtr->zobrist = 0;

    UNumspeed cursor = makeUNumspeed(0);

    forRawYCoords(y) {
        forRawXCoords(x) {
            if (fen[cursor] <= '8') {
                x += UNumspeed(fen[cursor] - '1');
            } else {
                const auto piece = Piece::fromChar(fen[cursor]);
                const auto coord = Coord::fromRaw(x, y);
				setPiece<true>(board, piece, coord);
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

void Board::setFromFen(Type &board, const std::string fen) {
    std::stringstream fenStream(fen);

	std::string positionPart, turnPart, castlePart, enapassantPart;

	fenStream >> positionPart;
	fenStream >> turnPart;
	fenStream >> castlePart;
	fenStream >> enapassantPart;

    setPositionFromFen(board, positionPart);
    board.turn = Color::fromString(turnPart);
    castle(board, Castle::fromString(castlePart));
    enpassant(board, Enpassant::fromString(enapassantPart));

    fenStream >> board.depthPtr->halfmoveClock;
    fenStream >> board.initialFullmoveNumber;
}

std::string Board::toFen(const Type &board) {
    std::stringstream resultStream;

    resultStream << getFenPosition(board) << " ";
    resultStream << Color::show(board.turn) << " ";
    resultStream << Castle::show(Board::castle(board)) << " ";
    resultStream << Enpassant::show(Board::enpassant(board)) << " ";
    resultStream << board.depthPtr->halfmoveClock << " ";
    resultStream << Board::fullMoveNumber(board);

    return resultStream.str();
}

std::string Board::show(const Type& board) {
    std::string result;
    result.reserve(64+8);

    forRawYCoords(y) {
        forRawXCoords(x) {
            result += Piece::show(board.squares[x|y]);
        }

        result += "\n";
    }

    return result;
}
