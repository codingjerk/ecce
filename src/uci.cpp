#include "uci.hpp"

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>

#include "boards.hpp"
#include "colors.hpp"
#include "tests.hpp"
#include "moves.hpp"
#include "mover.hpp"
#include "perft.hpp"

// @TODO: Move to info.{h,c}pp
namespace Info {
    const std::string name    = "Ecce";
    const std::string version = "rev. 200";
    const std::string author  = "D. Gruzdev";
}

// @TODO: Move to options.{h,c}pp
namespace Options {
    void show() {
        // Empty
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

    if (*cursor == "fen") {
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
    } else if (*cursor == "startpos") {
        ++cursor;
        Board::setFromFen(Board::master, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    if (*cursor == "moves") {
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

    if (*cursor == "complex") {
        Tests::runAll(true);
    } else {
        Tests::runAll();
    }

    return true;
}

bool perft(std::list<std::string> arguments) {
    auto cursor = arguments.begin();

    std::stringstream ss(*cursor);
    unsigned long long depth;
    ss >> depth;

    Move::Buffer *moves = new Move::Buffer[MAX_DEPTH];
    Perft::perft(moves, Board::master, depth);

    return true;
}

bool board(std::list<std::string>) {
    std::cout << Board::show(Board::master);

    return true;
}

bool stop(std::list<std::string>) {
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
    result["exit"]       = &exit;
    result["quit"]       = &exit;
    result["stop"]       = &stop;
    result["test"]       = &test;
    result["perft"]      = &perft;

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
