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

int Tests::runAll(UNumspeed complexity) {    
    SECTION(Framework);
    CHECK(true);

    SECTION(Coords);
    CHECK(Coord::fromString("a1") == 0);
    CHECK(Coord::show(0ull) == "a1");
    CHECK(Coord::show(Coord::fromString("h8")) == "h8");
    CHECK(Coord::show(Coord::fromRaw(7,56)) == "h8");
    CHECK(Coord::show(Coord::fromRaw(1,0)) == "b1");

    SECTION(Moves);
    Move::initTables();
    CHECK(Move::fromString("a1a1") == 0);
    CHECK(Move::show(0) == "a1a1");
    CHECK(Move::show(Move::fromString("a8h1")) == "a8h1");

    CHECK(Move::isCapture(Move::create(1,2)) == false);

    CHECK(Move::isCapture(Move::promotion(1,2, Piece::create(Black, Knight), Piece::create(White, Pawn))));

    SECTION(Bitboards);
    Bitboard::initTables();
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("b2"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n01000000\n00000000\n");
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("a1"))) == "00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n10000000\n");
    CHECK(Bitboard::show(Bitboard::fromCoord(Coord::fromString("h8"))) == "00000001\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n00000000\n");

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

	CHECK(Board::ply(board) == 0);
	board.depthPtr++;
	CHECK(Board::ply(board) == 1);
	board.depthPtr++;
	CHECK(Board::ply(board) == 2);
	board.depthPtr -= 2;

    SECTION(Generator);
    Generator::initTables();
    Move::Buffer buffer;

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
    for (unsigned int i = 1; i <= total; ++i) {
        Move::make(buffer[i], board);
        Move::unmake(buffer[i], board);
        CHECK(Board::toFen(board) == Board::toFen(boardMain));
    }

    if (complexity >= 1) {
        Move::Buffer *moves = new Move::Buffer[MAX_DEPTH];

        Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        CHECK(Perft::perft_quiet(moves, board, 1) == 20);
        CHECK(Perft::perft_quiet(moves, board, 2) == 400);
        CHECK(Perft::perft_quiet(moves, board, 3) == 8902);
        CHECK(Perft::perft_quiet(moves, board, 4) == 197281);
        CHECK(Perft::perft_quiet(moves, board, 5) == 4865609);
        CHECK(Perft::perft_quiet(moves, board, 6) == 119060324);
		if (complexity >= 2) CHECK(Perft::perft(moves, board, 7) == 3195901860);
        if (complexity >= 3) CHECK(Perft::perft(moves, board, 8) == 84998978956);

        Board::setFromFen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        CHECK(Perft::perft_quiet(moves, board, 1) == 48);
        CHECK(Perft::perft_quiet(moves, board, 2) == 2039);
        CHECK(Perft::perft_quiet(moves, board, 3) == 97862);
        CHECK(Perft::perft_quiet(moves, board, 4) == 4085603);
        CHECK(Perft::perft_quiet(moves, board, 5) == 193690690);

        Board::setFromFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        CHECK(Perft::perft_quiet(moves, board, 1) == 14);
        CHECK(Perft::perft_quiet(moves, board, 2) == 191);
        CHECK(Perft::perft_quiet(moves, board, 3) == 2812);
        CHECK(Perft::perft_quiet(moves, board, 4) == 43238);
        CHECK(Perft::perft_quiet(moves, board, 5) == 674624);
        CHECK(Perft::perft_quiet(moves, board, 6) == 11030083);
        if (complexity >= 2) CHECK(Perft::perft_quiet(moves, board, 7) == 178633661);

        Board::setFromFen(board, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        CHECK(Perft::perft_quiet(moves, board, 1) == 6);
        CHECK(Perft::perft_quiet(moves, board, 2) == 264);
        CHECK(Perft::perft_quiet(moves, board, 3) == 9467);
        CHECK(Perft::perft_quiet(moves, board, 4) == 422333);
        CHECK(Perft::perft_quiet(moves, board, 5) == 15833292);

        Board::setFromFen(board, "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq - 0 6");
        CHECK(Perft::perft_quiet(moves, board, 1) == 42);
        CHECK(Perft::perft_quiet(moves, board, 2) == 1352);
        CHECK(Perft::perft_quiet(moves, board, 3) == 53392);

        Board::setFromFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
        CHECK(Perft::perft_quiet(moves, board, 1) == 46);
        CHECK(Perft::perft_quiet(moves, board, 2) == 2079);
        CHECK(Perft::perft_quiet(moves, board, 3) == 89890);
        CHECK(Perft::perft_quiet(moves, board, 4) == 3894594);
        if (complexity >= 2) CHECK(Perft::perft_quiet(moves, board, 5) == 164075551);
    }

    SECTION(UCI);
    // Silent
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move::Type silent = Move::fromString("b1c3", board);
    Move::make(silent, board);
    Color::invert(board.turn);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR b KQkq - 0 1");
    Color::invert(board.turn);
    Move::unmake(silent, board);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    //Capture
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move::Type capture = Move::fromString("d1d7", board);
    Move::make(capture, board);
    Color::invert(board.turn);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppQpppp/8/8/8/8/PPPPPPPP/RNB1KBNR b KQkq - 0 1");
    Color::invert(board.turn);
    Move::unmake(capture, board);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    //PawnDouble
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move::Type pawnDouble = Move::fromString("e2e4", board);
    Move::make(pawnDouble, board);
    Color::invert(board.turn);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    Color::invert(board.turn);
    Move::unmake(pawnDouble, board);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    //Promotion
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move::Type promotion = Move::fromString("e2e5Q", board);
    Move::make(promotion, board);
    Color::invert(board.turn);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/4Q3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    Color::invert(board.turn);
    Move::unmake(promotion, board);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    //Enpassant
    Board::setFromFen(board, "r1bqkbnr/ppppppp1/n7/6Pp/8/8/PPPPPP1P/RNBQKBNR w KQkq h6 0 3");
    Move::Type enpassant = Move::fromString("g5h6", board);
    Move::make(enpassant, board);
    Color::invert(board.turn);
    CHECK(Board::toFen(board) == "r1bqkbnr/ppppppp1/n6P/8/8/8/PPPPPP1P/RNBQKBNR b KQkq - 0 3");
    Color::invert(board.turn);
    Move::unmake(enpassant, board);
    CHECK(Board::toFen(board) == "r1bqkbnr/ppppppp1/n7/6Pp/8/8/PPPPPP1P/RNBQKBNR w KQkq h6 0 3");

    //Castle-Long
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    Move::Type castleLong = Move::fromString("e1c1", board);
    Move::make(castleLong, board);
    Color::invert(board.turn);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 0 1");
    Color::invert(board.turn);
    Move::unmake(castleLong, board);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");

    //Castle-Short
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    Move::Type castleShort = Move::fromString("e1g1", board);
    Move::make(castleShort, board);
    Color::invert(board.turn);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 0 1");
    Color::invert(board.turn);
    Move::unmake(castleShort, board);
    CHECK(Board::toFen(board) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");

    SECTION(Evaluation and Score);
    Board::setFromFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(Eval::material<White>(board) == Score::Draw);
    CHECK(Eval::material<Black>(board) == Score::Draw);
    CHECK(Eval::positional<White>(board) == Score::Draw);
    CHECK(Eval::positional<Black>(board) == Score::Draw);

    Board::removePiece(board, Coord::E2);
    CHECK(Eval::material<White>(board) == -Score::Pawn);
    CHECK(Eval::material<Black>(board) == Score::Pawn);

    Board::removePiece(board, Coord::A8);
    CHECK(Eval::material<White>(board) == Score::Rook - Score::Pawn);
    CHECK(Eval::material<Black>(board) == Score::Pawn - Score::Rook);

    RESULTS;
}
