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

    Move::Buffer *moves = new Move::Buffer[MAX_DEPTH];
	unsigned long int start = GetTickCount();
    UNummax nodes = Perft::perft(moves, Board::master, depth);
	unsigned long int total = GetTickCount() - start;
	std::cout << "Total time: " << total << "ms (" << total / 1000.0 << "s)\n";
	std::cout << "NPS: " << nodes / total << "K nodes per second.\n";
    delete[] moves;

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
        Numspeed depth;
        ss >> depth;

        auto tm = TM::depth(depth);
        auto bm = Search::incremental(Board::master, tm);

        std::cout << "bestmove " << Move::show(bm) << "\n";
    } else {
        std::cout << "This go command is doesn't support yet.\n";
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
