#include "tests.hpp"

#include "cstf.hpp"

#include "coords.hpp"
#include "moves.hpp"
#include "bitboards.hpp"
#include "pieces.hpp"
#include "boards.hpp"
#include "generator.hpp"
#include "mover.hpp"
#include "checker.hpp"
#include "perft.hpp"
#include "score.hpp"
#include "eval.hpp"
#include "PST.hpp"
#include "utils.hpp"

void framework(UNumspeed complexity) {
    SECTION(Framework);
    CHECK(true);
}

void coords(UNumspeed complexity) {
    SECTION(Coords);
    CHECK(Coord::fromString("a1") == 0);
    CHECK(Coord::show(0ull) == "a1");
    CHECK(Coord::show(Coord::fromString("h8")) == "h8");
    CHECK(Coord::show(Coord::fromRaw(7, 56)) == "h8");
    CHECK(Coord::show(Coord::fromRaw(1, 0)) == "b1");
}

void moves(UNumspeed complexity) {
    SECTION(Moves);
    CHECK(Move::fromString("a1a1") == 0);
    CHECK(Move::show(0) == "a1a1");
    CHECK(Move::show(Move::fromString("a8h1")) == "a8h1");

    CHECK(Move::isCapture(Move::create(1, 2)) == false);

    CHECK(Move::isCapture(Move::promotion(1, 2, Piece::create(Black, Knight), Piece::create(White, Pawn))));

    Board::Type board;
    Board::setFromFen(board, "1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - - 0 0");
    CHECK(Move::fromShortString<Black>("Qd1+", board) == Move::fromString("d6d1", board));

    Board::setFromFen(board, "3r1k2/4npp1/1ppr3p/p6P/P2PPPP1/1NR5/5K2/2R5 w - - 0 0");
    CHECK(Move::fromShortString<White>("d5", board) == Move::fromString("d4d5", board));

    Board::setFromFen(board, "2q1rr1k/3bbnnp/p2p1pp1/2pPp3/PpP1P1P1/1P2BNNP/2BQ1PRK/7R b - - 0 0");
    CHECK(Move::fromShortString<Black>("f5", board) == Move::fromString("f6f5", board));

    Board::setFromFen(board, "rnbqkb1r/p3pppp/1p6/2ppP3/3N4/2P5/PPP1QPPP/R1B1KB1R w KQkq - 0 0");
    CHECK(Move::fromShortString<White>("e6", board) == Move::fromString("e5e6", board));

    Board::setFromFen(board, "r1b2rk1/2q1b1pp/p2ppn2/1p6/3QP3/1BN1B3/PPP3PP/R4RK1 w - - 0 0");
    CHECK(Move::fromShortString<White>("Nd5", board) == Move::fromString("c3d5", board));
    CHECK(Move::fromShortString<White>("Na4", board) == Move::fromString("c3a4", board));

    Board::setFromFen(board, "rnbq1rk1/pppp1ppp/4pn2/8/1bPP4/P1N5/1PQ1PPPP/R1B1KBNR b KQ - 1 5");
    CHECK(Move::fromShortString<Black>("Bcx3+", board) == Move::fromString("b4c3", board));

    Board::setFromFen(board, "r4rk1/3nppbp/bq1p1np1/2pP4/8/2N2NPP/PP2PPB1/R1BQR1K1 b - - 1 12");
    CHECK(Move::fromShortString<Black>("Rfb8", board) == Move::fromString("f8b8", board));

    Board::setFromFen(board, "r2q1rk1/2p1bppp/p2p1n2/1p2P3/4P1b1/1nP1BN2/PP3PPP/RN1QR1K1 w - - 1 12");
    CHECK(Move::fromShortString<White>("exf6", board) == Move::fromString("e5f6", board));

    Board::setFromFen(board, "r2qkbnr/2p2pp1/p1pp4/4p2p/4P1b1/5N1P/PPPP1PP1/RNBQ1RK1 w kq - 1 8");
    CHECK(Move::fromShortString<White>("hxg4", board) == Move::fromString("h3g4", board));

    Board::setFromFen(board, "rn1qkb1r/pp2pppp/5n2/3p1b2/3P4/1QN1P3/PP3PPP/R1B1KBNR b KQkq - 1 1");
    CHECK(Move::fromShortString<Black>("Bc8", board) == Move::fromString("f5c8", board));
}

void bitboards(UNumspeed complexity) {
    SECTION(Bitboards);
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("b2"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n01000000\n00000000\n");
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("a1"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n10000000\n");
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("h8"))) == "00000001\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n");
}

void colors(UNumspeed complexity) {
    SECTION(Colors);
    CHECK(Color::fromString("w") == White);
    CHECK(Color::show(Black) == "b");
}

void pieces(UNumspeed complexity) {
    SECTION(Pieces);
    CHECK(Piece::fromString("Q") == Piece::create(White, Queen));
    CHECK(Piece::show(Piece::fromString("p")) == "p");
}

void enpassants(UNumspeed complexity) {
    SECTION(Enpassants);
    CHECK(Enpassant::fromString("-") == Enpassant::null);
    CHECK(Enpassant::fromString("f4") == Coord::fromString("f4"));
}

void castles(UNumspeed complexity) {
    SECTION(Castles);
    CHECK(Castle::fromString("-") == Castle::null);
    CHECK(Castle::fromString("KQ") == (Castle::whiteKing | Castle::whiteQueen));
}

void boards(UNumspeed complexity) {
    SECTION(Boards);
    Board::Type board;
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    CHECK(Board::ply(board) == 0);
    board.depthPtr++;
    CHECK(Board::ply(board) == 1);
    board.depthPtr++;
    CHECK(Board::ply(board) == 2);
    board.depthPtr -= 2;
}

void generator(UNumspeed complexity) {
    SECTION(Generator);
    Board::Type board;
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    auto &buffer = Board::currentBuffer(board);

    buffer[0] = 0;
    Generator::knights<Black>(buffer, board);
    auto total = buffer[0];
    CHECK(total == 4);

    buffer[0] = 0;
    Generator::kings<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 1);

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    buffer[0] = 0;
    Generator::kings<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 4);

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    buffer[0] = 0;
    Generator::pawns<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 16);

    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/2B1P3/N2P1N2/PPPBQPPP/R3K2R w KQkq - 0 1");
    buffer[0] = 0;
    Generator::pawns<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 9);

    Board::setFromFen(board, "rn2k2r/pp2p3/2p2pb1/3p1n1p/2B1P1b1/Nq1P1NP1/PPPBQP1P/R3K2R w KQkq - 0 1");
    CHECK(Board::toFen(board) == "rn2k2r/pp2p3/2p2pb1/3p1n1p/2B1P1b1/Nq1P1NP1/PPPBQP1P/R3K2R w KQkq - 0 1");
    buffer[0] = 0;
    Generator::pawns<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 9);

    buffer[0] = 0;
    Generator::pawns<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 11);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    buffer[0] = 0;
    Generator::bishops<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 10);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    buffer[0] = 0;
    Generator::bishops<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 6);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    buffer[0] = 0;
    Generator::rooks<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 8);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    buffer[0] = 0;
    Generator::rooks<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 3);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 w - - 0 1");
    buffer[0] = 0;
    Generator::queens<White>(buffer, board);
    total = buffer[0];
    CHECK(total == 7);

    Board::setFromFen(board, "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    CHECK(Board::toFen(board) == "r1bq2rk/pp3pbp/2p1p1pQ/7P/3P4/2PB1N2/PP3PPR/2KR4 b - - 0 1");
    buffer[0] = 0;
    Generator::queens<Black>(buffer, board);
    total = buffer[0];
    CHECK(total == 13);

    // Perft (with unlegals) at depth 1 tests
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Generator::phase(buffer, board);
    total = buffer[0];
    CHECK(total == 20);

    Board::setFromFen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    Generator::phase(buffer, board);
    total = buffer[0];
    CHECK(total == 48);

    Board::setFromFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    Generator::phase(buffer, board);
    total = buffer[0];
    CHECK(total == 16);

    Board::setFromFen(board, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    Generator::phase(buffer, board);
    total = buffer[0];
    CHECK(total == 38);

    Board::setFromFen(board, "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6");
    Generator::phase(buffer, board);
    total = buffer[0];
    CHECK(total == 42);

    if (complexity >= 1) {
        Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        CHECK(Perft::perft_quiet(board, 1) == 20);
        CHECK(Perft::perft_quiet(board, 2) == 400);
        CHECK(Perft::perft_quiet(board, 3) == 8902);
        CHECK(Perft::perft_quiet(board, 4) == 197281);
        CHECK(Perft::perft_quiet(board, 5) == 4865609);
        CHECK(Perft::perft_quiet(board, 6) == 119060324);
        if (complexity >= 2) CHECK(Perft::perft_quiet(board, 7) == 3195901860);
        if (complexity >= 3) CHECK(Perft::perft_quiet(board, 8) == 84998978956);

        Board::setFromFen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        CHECK(Perft::perft_quiet(board, 1) == 48);
        CHECK(Perft::perft_quiet(board, 2) == 2039);
        CHECK(Perft::perft_quiet(board, 3) == 97862);
        CHECK(Perft::perft_quiet(board, 4) == 4085603);
        CHECK(Perft::perft_quiet(board, 5) == 193690690);

        Board::setFromFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        CHECK(Perft::perft_quiet(board, 1) == 14);
        CHECK(Perft::perft_quiet(board, 2) == 191);
        CHECK(Perft::perft_quiet(board, 3) == 2812);
        CHECK(Perft::perft_quiet(board, 4) == 43238);
        CHECK(Perft::perft_quiet(board, 5) == 674624);
        CHECK(Perft::perft_quiet(board, 6) == 11030083);
        if (complexity >= 2) CHECK(Perft::perft_quiet(board, 7) == 178633661);

        Board::setFromFen(board, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        CHECK(Perft::perft_quiet(board, 1) == 6);
        CHECK(Perft::perft_quiet(board, 2) == 264);
        CHECK(Perft::perft_quiet(board, 3) == 9467);
        CHECK(Perft::perft_quiet(board, 4) == 422333);
        CHECK(Perft::perft_quiet(board, 5) == 15833292);

        Board::setFromFen(board, "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6");
        CHECK(Perft::perft_quiet(board, 1) == 42);
        CHECK(Perft::perft_quiet(board, 2) == 1352);
        CHECK(Perft::perft_quiet(board, 3) == 53392);

        Board::setFromFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
        CHECK(Perft::perft_quiet(board, 1) == 46);
        CHECK(Perft::perft_quiet(board, 2) == 2079);
        CHECK(Perft::perft_quiet(board, 3) == 89890);
        CHECK(Perft::perft_quiet(board, 4) == 3894594);
        if (complexity >= 2) CHECK(Perft::perft_quiet(board, 5) == 164075551);
    }
}

void evaluation(UNumspeed complexity) {
    SECTION(Evaluation);
    Board::Type board;
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(Eval::material<White>(board) == Score::Draw);
    CHECK(Eval::material<Black>(board) == Score::Draw);
    CHECK(Eval::positional<White>(board) == Score::Draw);
    CHECK(Eval::positional<Black>(board) == Score::Draw);

    Board::removePiece<false>(board, Coord::E2);
    CHECK(Eval::material<White>(board) == -Score::Pawn);
    CHECK(Eval::material<Black>(board) == Score::Pawn);

    Board::removePiece<false>(board, Coord::A8);
    CHECK(Eval::material<White>(board) == Score::Rook - Score::Pawn);
    CHECK(Eval::material<Black>(board) == Score::Pawn - Score::Rook);
}

int Tests::runAll(UNumspeed complexity) {
    framework(complexity);
    coords(complexity);
    moves(complexity);
    bitboards(complexity);
    colors(complexity);
    pieces(complexity);
    enpassants(complexity);
    castles(complexity);
    boards(complexity);
    generator(complexity);
    evaluation(complexity);

    RESULTS;
}
