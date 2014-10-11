#include "zobrist.hpp"

#include "rand.hpp"

Zobrist::Type Zobrist::table[makeUNumspeed(1) << Piece::usedBits][makeUNumspeed(1) << Coord::usedBits];
Zobrist::Type Zobrist::turnKey;

void Zobrist::initTables() {
    turnKey = rand64();

    for (UNumspeed piece = 0; piece < 16; ++piece) {
        for (UNumspeed coord = 0; coord < 64; ++coord) {
            table[piece][coord] = rand64();
        }
    }
}
