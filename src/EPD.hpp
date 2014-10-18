#ifndef EPD_HPP
#define EPD_HPP

#include "core.hpp"

#include <sstream>

#include "boards.hpp"
#include "search.hpp"

namespace EPD {
    struct Record {
        Board::Type board;
        Move::Type bestmove;
    };

    inline void loadRecord(Record& record, std::string str) {
        std::stringstream ss(str);

        std::string position;
        ss >> position;

        std::string color;
        ss >> color;

        std::string castle;
        ss >> castle;

        std::string enpassant;
        ss >> enpassant;

        Board::setFromFen(record.board, position + " " + color + " " + castle + " " + enpassant + " 0 1");

        while (!ss.eof()) {
            std::string cursor;
            ss >> cursor;

            if (cursor == "bm") {
                std::string bestmove;
                ss >> bestmove;
                if (bestmove[bestmove.size() - 1] == ';') {
                    bestmove.pop_back();
                }

                if (record.board.turn == White) {
                    record.bestmove = Move::fromShortString<White>(bestmove, record.board);
                } else {
                    record.bestmove = Move::fromShortString<Black>(bestmove, record.board);
                }

                break;
            }
        }
    }

    inline bool checkRecord(Record& record) {
        auto tm = TM::time(2000);
        auto bestmove = Search::incremental(record.board, tm);

        return bestmove == record.bestmove;
    }
}

#endif /* EPD_HPP */
