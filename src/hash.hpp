#ifndef HASH_HPP
#define HASH_HPP

#include "zobrist.hpp"
#include "score.hpp"
#include "moves.hpp"
#include "boards.hpp"
#include "statistic.hpp"

namespace Hash {
    struct Node {
        enum Type { Invalid = 0, Alpha = 1, Beta = 2, Exact = 3 };

        Zobrist::Type key; // 64
        Move::Type bestMove: 24; // 88
        UNumspeed depth: 9; // 97
        Type type: 3; // 100
        Score::Type score: 28; // 128

        Node() : key(0), bestMove(0), depth(0), type(Invalid), score(0) {}
    };

    const Node Empty;

    extern Node *table;
    extern UNumspeed size;

    inline void setTableSize(UNumspeed newSize) {
        if (table) delete[] table;

        size = newSize;

        table = new Node[size];
    }

    inline void setTableSizeInMb(UNumspeed sizeInMb) {
        setTableSize(sizeInMb * 1024 * 1024 / sizeof(Node));
    }

    inline void write(Zobrist::Type key, Move::Type bestMove, Score::Type score, UNumspeed depth, Node::Type type, UNumspeed ply) {
        auto const halfKey = key % size;

        if (type >= table[halfKey].type && depth >= table[halfKey].depth) {
            if (key == table[halfKey].key || table[halfKey].type == Node::Invalid) {
                Statistic::hashGoodRewrited();
            } else {
                Statistic::hashBadRewrited();
            }

            table[halfKey].key      = key;
            table[halfKey].bestMove = bestMove;
            table[halfKey].depth    = depth;
            table[halfKey].type     = type;
            
            if (Score::isPositiveMate(score))      score += ply;
            else if (Score::isNegativeMate(score)) score -= ply;
            table[halfKey].score    = score;
        }
    }

    inline const Node &read(Zobrist::Type key) {
        auto const halfKey = key % size;

        if (key == table[halfKey].key) {
            return table[halfKey];
        } else {
            return Empty;
        }
    }

    inline void phase(Move::Buffer &buffer, const Board::Type &board) {
        const auto &node = read(board.depthPtr->zobrist);

        buffer[0] = (node.bestMove > 0);
        buffer[1] = node.bestMove;
    }

    inline void clear() {
        for (UNumspeed i = 0; i < size; ++i) {
            table[i] = Empty;
        }
    }

    inline UNumspeed fillFactor() {
        UNumspeed filled = 0;
        for (UNumspeed i = 0; i < size; ++i) {
            if ((table + i)->type != Node::Invalid) ++filled;
        }

        return filled * 1000 / size;
    }
}

#endif /* HASH_HPP */
