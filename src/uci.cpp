#include "uci.hpp"

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>

#include "boards.hpp"

// @TODO: Move to info.{h,c}pp
namespace Info {
    std::string name    = "Ecce";
    std::string version = "rev. 200";
    std::string author  = "D. Gruzdev";
}

bool exit(std::list<std::string>) {
    return false;
}

bool uci(std::list<std::string>) {
    std::cout << "id name " << Info::name << " " << Info::version << "\n";
    std::cout << "id author " << Info::author << "\n";

    // @TODO: out options

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
        ++cursor;

        std::cout << "@TODO: position moves isn't ready yet.\n";
    }

    return true;
}

bool go(std::list<std::string> arguments) {
    auto cursor = arguments.begin();

    if (*cursor == "depth") {

    } else {
        std::cout << "go depth command only supported yet.\n";
    }

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

std::map<std::string, ProcessFunction> uciFunctions = {
    {"uci",        &uci},
    {"debug",      &debug},
    {"isready",    &isready},
    {"setoption",  &setoption},
    {"ucinewgame", &ucinewgame},
    {"position",   &position},
    {"board",      &board},
    {"exit",       &exit},
    {"quit",       &exit},
    {"go",         &go},
    {"stop",       &stop}
};

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
