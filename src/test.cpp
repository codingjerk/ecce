#include "cstf.hpp"

#include "coords.hpp"
#include "moves.hpp"
#include "bitboards.hpp"
#include "pieces.hpp"
#include "boards.hpp"
#include "generator.hpp"
#include "mover.hpp"

const UNumspeed MAX_DEPTH = 1024;
UNum64 perft_nodes(Generator::MoveBuffer *buffer, Board::Type &board, UNumspeed depth) {
    if (depth == 0) return 1;
    UNum64 result = 0;

    Generator::forBoard(buffer[depth], board);
    UNumspeed total = buffer[depth][0];
    for (UNumspeed i = 1; i <= total; ++i) {
        Move::make(buffer[depth][i], board);
        result += perft_nodes(buffer, board, depth - 1);
        Move::unmake(buffer[depth][i], board);
    }

    return result;
}

UNum64 perft(Generator::MoveBuffer *buffer, Board::Type &board, UNumspeed depth) {
    if (depth == 0) return 1;
    UNum64 result = 0;

    Generator::forBoard(buffer[depth], board);
    UNumspeed total = buffer[depth][0];
    for (UNumspeed i = 1; i <= total; ++i) {
        Move::make(buffer[depth][i], board);
        const auto nodes = perft_nodes(buffer, board, depth - 1);;
        std::cout << "Move: " << Move::show(buffer[depth][i]) << " = " << nodes << "\n";
        result += nodes;
        Move::unmake(buffer[depth][i], board);
    }

    return result;
}

int main(int, char**) {
    SECTION(Framework);
    CHECK(true);
    //EXCEPTION(throw 1);

    SECTION(Coords);
    CHECK(Coord::fromString("a1") == 0);
    CHECK(Coord::show(0ull) == "a1");
    CHECK(Coord::show(Coord::fromString("h8")) == "h8");
    CHECK(Coord::show(Coord::fromRaw(7,56)) == "h8");
    CHECK(Coord::show(Coord::fromRaw(1,0)) == "b1");

    SECTION(Moves);
    CHECK(Move::fromString("a1a1") == 0);
    CHECK(Move::show(0) == "a1a1");
    CHECK(Move::show(Move::fromString("a8h1")) == "a8h1");

    CHECK(Move::isPromotion(Move::create(1,2)) == false);
    CHECK(Move::isCapture(Move::create(1,2)) == false);

    CHECK(Move::isPromotion(Move::promotion(1,2, Piece::create(Black, Knight), Piece::create(White, Pawn))));
    CHECK(Move::isCapture(Move::promotion(1,2, Piece::create(Black, Knight), Piece::create(White, Pawn))));

    SECTION(Bitboards);
    Bitboard::initTables();
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("b2"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n01000000\n00000000\n");
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("a1"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n10000000\n");
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("h8"))) == "00000001\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n");
    CHECK(Bitboard::show(Bitboard::fromMove(Move::fromString("a1b2"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n01000000\n10000000\n");

    SECTION(Colors);
    Color::initTables();
    CHECK(Color::fromString("w") == White);
    CHECK(Color::show(Black) == "b");

    SECTION(Pieces);
    Piece::initTables();
    CHECK(Piece::fromString("Q") == Piece::create(White, Queen));
    CHECK(Piece::show(Piece::fromString("p")) == "p");

    SECTION(Enpassants);
    CHECK(Enpassant::fromString("-") == Enpassant::null);
    CHECK(Enpassant::fromString("f4") == Coord::fromString("f4"));

    SECTION(Castles);
    CHECK(Castle::fromString("-") == Castle::null);
    CHECK(Castle::fromString("KQ") == (Castle::whiteKing | Castle::whiteQueen));

    SECTION(Boards);
    Board::Type board; 
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    SECTION(Generator);
    Generator::initTables();
    Generator::MoveBuffer buffer;

    buffer[0] = 0;
    Generator::forKnights<Black>(buffer, board);
    auto total = buffer[0]; 
    CHECK(total == 4);

    buffer[0] = 0;
    Generator::forKings<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 1);

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    buffer[0] = 0;
    Generator::forKings<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 4);

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    buffer[0] = 0;
    Generator::forPawns<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 16);

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    buffer[0] = 0;
    Generator::forPawns<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 9);

    Board::setFromFen(board, "rn2k2r/pp2p3/2p2pb1/3p1n1p/2B1P1b1/Nq1P1NP1/PPPBQP1P/R3K2R w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rn2k2r/pp2p3/2p2pb1/3p1n1p/2B1P1b1/Nq1P1NP1/PPPBQP1P/R3K2R w KQkq - 0 1");
    buffer[0] = 0;
    Generator::forPawns<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 9);

    buffer[0] = 0;
    Generator::forPawns<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 11);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    buffer[0] = 0;
    Generator::forBishops<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 10);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    buffer[0] = 0;
    Generator::forBishops<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 6);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    buffer[0] = 0;
    Generator::forRooks<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 8);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    buffer[0] = 0;
    Generator::forRooks<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 3);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    buffer[0] = 0;
    Generator::forQueens<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 7);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    buffer[0] = 0;
    Generator::forQueens<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 13);

    // Perft (with unlegals) at depth 1 tests
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Generator::forBoard(buffer, board);
    total = buffer[0];
    CHECK(total == 20);

    Board::setFromFen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    Generator::forBoard(buffer, board);
    total = buffer[0];
    CHECK(total == 48);

    Board::setFromFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    Generator::forBoard(buffer, board);
    total = buffer[0];
    CHECK(total == 16);

    Board::setFromFen(board, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    Generator::forBoard(buffer, board);
    total = buffer[0];
    CHECK(total == 38);

    Board::setFromFen(board, "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6");
    Generator::forBoard(buffer, board);
    total = buffer[0];
    CHECK(total == 42);

    Board::setFromFen(board, "2br2k1/2q3rn/p2NppQ1/2p1P3/Pp5R/4P3/1P3PPP/3R2K1 w - - 0 1");
    const auto boardMain = board;
    buffer[0] = 0;
    Generator::forBoard(buffer, board);
    total = buffer[0];
    for (int i = 1; i <= total; ++i) {
        Move::make(buffer[i], board);
        Move::unmake(buffer[i], board);
        CHECK(Board::toFen(board) == Board::toFen(boardMain));
    }

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Generator::MoveBuffer *moves = new Generator::MoveBuffer[MAX_DEPTH];
    for (int depth = 4; depth <= 4; ++depth) {
        const auto pr = perft(moves, board, depth);
        std::cout << "Perft at depth " << depth << " = " << pr << "\n";
    }

    RESULTS;
}
