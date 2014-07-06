#include "cstf.hpp"

#include "coords.hpp"
#include "moves.hpp"
#include "bitboards.hpp"
#include "pieces.hpp"
#include "boards.hpp"

int main(int, char**) {
    SECTION(Framework);
    CHECK(true);
    EXCEPTION(throw 1);

    SECTION(Coords);
    CHECK(Coord::fromString("a1") == 0);
    CHECK(Coord::show(0ull) == "a1");
    CHECK(Coord::show(Coord::fromString("h8")) == "h8");

    SECTION(Moves);
    CHECK(Move::fromString("a1a1") == 0);
    CHECK(Move::show(0) == "a1a1");
    CHECK(Move::show(Move::fromString("a8h1")) == "a8h1");

    SECTION(Bitboards);
    Bitboard::initTables();
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("b2"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n01000000\n00000000\n");
    CHECK(Bitboard::show(Bitboard::fromMove(Move::fromString("a1b2"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n01000000\n10000000\n");

    SECTION(Pieces);
    Piece::initTables();
    CHECK(Piece::fromString("Q") == Piece::create(White, Queen));
    CHECK(Piece::show(Piece::fromString("p")) == "p");

    SECTION(Boards);
    Board::Type board; 
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    RESULTS;
}
