#ifndef EPD_HPP
#define EPD_HPP

#include "core.hpp"

#include <sstream>
#include <string>

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

    inline bool checkRecord(Record& record, TM::TimeLimit tm) {
        auto bestmove = Search::incremental(record.board, tm);
        auto result = (bestmove == record.bestmove);

        if (!result) {
            std::cout << "Needed " << Move::show(record.bestmove) << ", but result is " << Move::show(bestmove) << "\n";
        }

        return result;
    }

    inline void checkFile(std::string epdContent, UNumspeed time) {
        EPD::Record record;
        auto cursor = epdContent.begin();

        UNummax total = 0;
        UNummax succes = 0;
        UNummax failed = 0;
        while (cursor != epdContent.end()) {
            std::string epdRecordAsText;

            while (cursor != epdContent.end()) {
                epdRecordAsText.push_back(*cursor);
                ++cursor;
                if (*cursor == '\n') {
                    ++cursor; 
                    break;
                }
            }

            EPD::loadRecord(record, epdRecordAsText);
            ++total;
            if (EPD::checkRecord(record, TM::time(time))) {
                ++succes;
                std::cout << "Succes\n";
            } else {
                ++failed;
                std::cout << "Failed\n";
            }
        }

        std::cout << "\nTotal: " << total << ", Succes: " << succes << "/" << total << "\n";
    }
}

#endif /* EPD_HPP */
