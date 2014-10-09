#include "uci.hpp"

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>

#include "boards.hpp"
#include "colors.hpp"
#include "moves.hpp"
#include "mover.hpp"

#include "tests.hpp"
#include "perft.hpp"
#include "tm.hpp"
#include "search.hpp"

#include "info.hpp"
#include "options.hpp"

namespace Move {
    inline Type fromString(const std::string text, Board::Type &board) {
        Move::Type simple = fromString(text);

        // @TODO: Create methods Move::from and Move::to
        Coord::Type from = (simple >> Coord::usedBits) & Coord::typeMask; 
        Coord::Type to = simple & Coord::typeMask;

        // Captures
        simple |= (board.squares[to] << captureOffset);

        // PawnDoubles
        if (board.squares[from] == Piece::create(White, Pawn) || board.squares[from] == Piece::create(Black, Pawn)) {
            if (from - to == 16 || to - from == 16) {
                simple |= (pawnDoubleFlag << specialOffset);
            }//v
            // v
            // Promotions
            else if (text.size() == 5) {
                Color::Type we = (board.squares[from]) & Color::typeMask;
                Piece::Type promoted = (Piece::fromChar(text[4]) & Dignity::typeMask) | we;
                simple |= (promoted << promotionOffset)
                       |  (promotionFlag << specialOffset);
            }//v
            // v
            // Enpassants
            else if (to == Board::enpassant(board)) {
                simple |= (enpassantFlag << specialOffset);
            }
        }//v
        // v
        // Castles
        else if (board.squares[from] == Piece::create(White, King) || board.squares[from] == Piece::create(Black, King)) {
            if ((from == Coord::E1 && to == Coord::C1) || (from == Coord::E8 && to == Coord::C8)) {
                simple |= (castleLongFlag << specialOffset);
            } else if ((from == Coord::E1 && to == Coord::G1) || (from == Coord::E8 && to == Coord::G8)) {
                simple |= (castleShortFlag << specialOffset);
            }
        }

        return simple;
    }
}

bool exit(std::list<std::string>) {
    return false;
}

bool uci(std::list<std::string>) {
    std::cout << "id name " << Info::name << " " << Info::version << "\n";
    std::cout << "id author " << Info::author << "\n";

    Options::show();

    std::cout << "uciok\n" << std::flush;

    return true;
}

bool debug(std::list<std::string>) {
    std::cout << "debug mode isn't supported yet.\n" << std::flush;

    return true;
}

bool isready(std::list<std::string>) {
    std::cout << "readyok\n" << std::flush;

    return true;
}

bool setoption(std::list<std::string>) {
    std::cout << "setoption command isn't supported yet.\n" << std::flush;

    return true;
}

bool ucinewgame(std::list<std::string>) {
    return true;
}

bool position(std::list<std::string> arguments) {
    auto cursor = arguments.begin();

    if (cursor != arguments.end() && *cursor == "fen") {
        ++cursor;
        std::string position = *cursor;
        ++cursor;
        std::string turn = *cursor;
        ++cursor;
        std::string castle = *cursor;
        ++cursor;
        std::string enpassant = *cursor;
        ++cursor;
        std::string halfmoveClock = *cursor;
        ++cursor;
        std::string totalMoves = *cursor;
        ++cursor;

        std::string fen = position + " " + turn + " " + castle + " " + enpassant + " "
                        + halfmoveClock + " " + totalMoves;

        Board::setFromFen(Board::master, fen);
    } else if (cursor != arguments.end() && *cursor == "startpos") {
        ++cursor;
        Board::setFromFen(Board::master, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    if (cursor != arguments.end() && *cursor == "moves") {
        for (++cursor; cursor != arguments.end(); ++cursor) {
            Move::Type move = Move::fromString(*cursor, Board::master);
            Move::make(move, Board::master);
            Color::invert(Board::master.turn);
            ++Board::master.depthOffset;
        }
    }

    return true;
}

bool test(std::list<std::string> arguments) {
    auto cursor = arguments.begin();
    
    UNumspeed complexity = 0;
    if (cursor != arguments.end()) {
        std::stringstream ss(*cursor);
        ss >> complexity;
    }
    
    unsigned long int start = GetTickCount();
    Tests::runAll(complexity);
    unsigned long int total = GetTickCount() - start;
    std::cout << "Total time: " << total << "ms (" << total / 1000.0 << "s)\n";

    return true;
}

bool perft(std::list<std::string> arguments) {
    auto cursor = arguments.begin();

    std::stringstream ss(*cursor);
    unsigned long long depth;
    ss >> depth;

    unsigned long int start = GetTickCount();
    UNummax nodes = Perft::perft(Board::master, depth);
    unsigned long int total = GetTickCount() - start;
    std::cout << "Total time: " << total << "ms (" << total / 1000.0 << "s)\n";
    std::cout << "NPS: " << nodes / total << "K nodes per second.\n";

    return true;
}

bool speed(std::list<std::string>) {
    unsigned long int start = GetTickCount();
    Board::Type testBoard;
    Board::setFromFen(testBoard, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    UNummax nodes = Perft::perft_quiet(testBoard, 6);
    unsigned long int total = GetTickCount() - start;
    std::cout << "Perft info - time: " << total << "ms (" << total / 1000.0 << "s), nodes: " << nodes << ", NPS: " << nodes / total << "K." << "\n";
    std::cout << "Score: " << nodes / total << "\n";

    Search::speed(testBoard);

    return true;
}

bool board(std::list<std::string>) {
    std::cout << Board::show(Board::master);

    return true;
}

bool fen(std::list<std::string>) {
    std::cout << Board::toFen(Board::master) << "\n";

    return true;
}

bool stop(std::list<std::string>) {
    return true;
}

bool go(std::list<std::string> arguments) {
    // @TODO: Write Commander class for process arguments
    auto cursor = arguments.begin();

    if (cursor != arguments.end() && *cursor == "depth") {
        ++cursor;
        std::stringstream ss(*cursor);
        UNumspeed depth;
        ss >> depth;
        depth = min(depth, MAX_DEPTH);

        auto tm = TM::depth(depth);
        auto bm = Search::incremental(Board::master, tm);

        std::cout << "bestmove " << Move::show(bm) << "\n";
    } else if (cursor == arguments.end() || *cursor == "infinite") {
        auto tm = TM::depth(MAX_DEPTH);
        auto bm = Search::incremental(Board::master, tm);

        std::cout << "bestmove " << Move::show(bm) << "\n";
    } else if (cursor != arguments.end() && *cursor == "movetime") {
        ++cursor;
        std::stringstream ss(*cursor);
        Numspeed time;
        ss >> time;

        auto tm = TM::time(time);
        auto bm = Search::incremental(Board::master, tm);

        std::cout << "bestmove " << Move::show(bm) << "\n";
    } else {
        Numspeed wtime = 0;
        Numspeed btime = 0;
        Numspeed winc  = 0;
        Numspeed binc  = 0;
        Numspeed movestogo = 0;

        while (cursor != arguments.end()) {
            if (*cursor == "wtime") {
                ++cursor;
                std::stringstream ss(*cursor);
                ss >> wtime;
            } else if (*cursor == "btime") {
                ++cursor;
                std::stringstream ss(*cursor);
                ss >> btime;
            } else if (*cursor == "winc") {
                ++cursor;
                std::stringstream ss(*cursor);
                ss >> winc;
            } else if (*cursor == "binc") {
                ++cursor;
                std::stringstream ss(*cursor);
                ss >> binc;
            } else if (*cursor == "movestogo") {
                ++cursor;
                std::stringstream ss(*cursor);
                ss >> movestogo;
            } else {
                std::cout << "This go command is doesn't support.\n";
                return true;
            }

            ++cursor;
        }


        TM::TimeLimit tm; 
        if (Board::master.turn == White) {
            tm = TM::time(wtime, winc, movestogo);
        } else {
            tm = TM::time(btime, binc, movestogo);
        }

        auto bm = Search::incremental(Board::master, tm);

        std::cout << "bestmove " << Move::show(bm) << "\n";
    }

    return true;
}

bool unknown(std::string command) {
    std::cout << "Unknown command: " << command << "\n"; 

    return true;
}

typedef bool (*ProcessFunction)(std::list<std::string>);

std::map<std::string, ProcessFunction> initUciFunctions() {
    std::map<std::string, ProcessFunction> result;

    result["uci"]        = &uci;
    result["debug"]      = &debug;
    result["isready"]    = &isready;
    result["setoption"]  = &setoption;
    result["ucinewgame"] = &ucinewgame;
    result["position"]   = &position;
    result["board"]      = &board;
    result["fen"]        = &fen;
    result["exit"]       = &exit;
    result["quit"]       = &exit;
    result["stop"]       = &stop;
    result["test"]       = &test;
    result["perft"]      = &perft;
    result["go"]         = &go;
    result["speed"]      = &speed;

    return result;
};

std::map<std::string, ProcessFunction> uciFunctions = initUciFunctions();

bool processCommand() {
    std::string line;
    std::getline(std::cin, line);

    std::stringstream stream(line);

    std::string command;
    stream >> command;

    std::list<std::string> arguments;
    while (stream.good()) {
        std::string arg;
        stream >> arg;

        arguments.push_back(arg);
    }

    if (uciFunctions.find(command) != uciFunctions.end()) {
        return uciFunctions[command](arguments);
    }
    
    return unknown(command);
}

void Uci::cycle() {
    Board::setFromFen(Board::master, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    while (processCommand());
}
