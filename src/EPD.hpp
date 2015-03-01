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
        Move::Type move;
        bool avoidMode;
        std::string description;
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

        bool moveSetted = false;
        while (!ss.eof()) {
            std::string cursor;
            ss >> cursor;

            if (cursor.size() > 2) {
                cursor = cursor.substr(cursor.size() - 2, 2);
            }

            if (!moveSetted && (cursor == "bm" || cursor == "am")) {
                moveSetted = true;
                record.avoidMode = (cursor == "am");

                std::string bestmove;
                ss >> bestmove;
                if (bestmove[bestmove.size() - 1] == ';') {
                    bestmove.pop_back();
                }

                if (record.board.turn == White) {
                    record.move = Move::fromShortString<White>(bestmove, record.board);
                } else {
                    record.move = Move::fromShortString<Black>(bestmove, record.board);
                }
            } else if (cursor == "id") {
                std::string description;
                ss >> description;

                if (description[0] == '\"') {
                    description = description.substr(1);
                }

                if (description[description.size() - 1] == ';') {
                    description = description.substr(0, description.size() - 2);
                } else if (description[description.size() - 1] == '\"') {
                    description = description.substr(0, description.size() - 1);
                }

                record.description = description;
            }
        }
    }

    inline bool checkRecord(Record& record, TM::DepthLimit DEPTH) {
        auto bestmove = Search::incremental(record.board, DEPTH);

        bool result = false;
        if (record.avoidMode) {
            result = (bestmove != record.move);

            if (!result) {
                std::cout << "Needed not " << Move::show(record.move) << ", but result is " << Move::show(bestmove) << "\n";
            }
        } else {
            result = (bestmove == record.move);

            if (!result) {
                std::cout << "Needed " << Move::show(record.move) << ", but result is " << Move::show(bestmove) << "\n";
            }
        }

        return result;
    }

    inline bool checkRecord(Record& record, TM::TimeLimit TIME) {
        auto bestmove = Search::incremental(record.board, TIME);

        bool result = false;
        if (record.avoidMode) {
            result = (bestmove != record.move);

            if (!result) {
                std::cout << "Needed not " << Move::show(record.move) << ", but result is " << Move::show(bestmove) << "\n";
            }
        } else {
            result = (bestmove == record.move);

            if (!result) {
                std::cout << "Needed " << Move::show(record.move) << ", but result is " << Move::show(bestmove) << "\n";
            }
        }

        return result;
    }

    // Returns epd with fail positions
    inline std::string checkFile(std::string epdContent, TM::DepthLimit DEPTH) {
        std::stringstream failedContent;
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
            if (EPD::checkRecord(record, DEPTH)) {
                ++succes;
                std::cout << total << " (" << record.description << ") >> Succes\n";
            }
            else {
                ++failed;
                std::cout << total << " (" << record.description << ") >> Failed\n";
                failedContent << Board::toFen(record.board) << "; " << (record.avoidMode ? "am " : "bm ")
                    << Move::show(record.move) << "; id \"" << record.description << "\";\n";
            }
        }

        std::cout << "\nTotal: " << total << ", Succes: " << succes << "/" << total << "\n";

        return failedContent.str();
    }

    inline std::string checkFile(std::string epdContent, TM::TimeLimit TIME) {
        std::stringstream failedContent;
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
            if (EPD::checkRecord(record, TIME)) {
                ++succes;
                std::cout << total << " (" << record.description << ") >> Succes\n";
            }
            else {
                ++failed;
                std::cout << total << " (" << record.description << ") >> Failed\n";
                failedContent << Board::toFen(record.board) << "; " << (record.avoidMode ? "am " : "bm ")
                    << Move::show(record.move) << "; id \"" << record.description << "\";\n";
            }
        }

        std::cout << "\nTotal: " << total << ", Succes: " << succes << "/" << total << "\n";

        return failedContent.str();
    }
}

#endif /* EPD_HPP */
