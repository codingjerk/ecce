#ifndef HASH_HPP
#define HASH_HPP

#include "zobrist.hpp"
#include "score.hpp"
#include "moves.hpp"
#include "boards.hpp"

namespace Hash {
    enum NodeType {
        Invalid = 0, 
        Alpha   = 1, 
        Beta    = 2, 
        Exact   = 3
    };

    struct Node {
        Zobrist::Type key   = 0;
        Move::Type bestMove = 0;
        UNumspeed depth       = 0;

        Score::Type score = 0;
        NodeType type = Invalid;
    };

    const Node empty;

    extern Node *table;
    extern UNumspeed size;
    extern UNumspeed modulo;

    inline void setTableSize(UNumspeed newSize) {
        if (table) delete[] table;

        size = newSize;
        modulo = size - 1;

        table = new Node[size];
    }

    inline void setTableSizeInMb(UNumspeed sizeInMb) {
        setTableSize(sizeInMb * 1024 * 1024 / sizeof(Node));
    }

    inline void write(Zobrist::Type key = 0, Move::Type bestMove = 0, Score::Type score = 0, UNumspeed depth = 0, NodeType type = Invalid) {
        auto const halfKey = key % modulo;

        if (type >= table[halfKey].type && depth >= table[halfKey].depth) {
            table[halfKey].key      = key;
            table[halfKey].bestMove = bestMove;
            table[halfKey].score    = score;
            table[halfKey].depth    = depth;
            table[halfKey].type     = type;
        }
    }

    inline const Node &read(Zobrist::Type key) {
        auto const halfKey = key % modulo;

        if (key == table[halfKey].key) {
            return table[halfKey];
        } else {
            return Hash::empty;
        }
    }

    inline void phase(Move::Buffer &buffer, const Board::Type &board) {
        const auto &node = read(board.depthPtr->zobrist);

        if (node.bestMove != 0) {
            buffer[1] = node.bestMove;
            buffer[0] = 1;
        } else {
            buffer[0] = 0;
        }
    }

    inline UNumspeed fillFactor() {
        if (!table || size == 0) return 0;

        UNumspeed filled = 0;
        for (UNumspeed i = 0; i < size; ++i) {
            if ((table + i)->type != Invalid) ++filled;
        }

        return filled * 1000 / size;
    }
}

#endif /* HASH_HPP */
