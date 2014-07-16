#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Generator {
    void initTables();

    template <Color::Type COLOR> void forPawns(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void forKnights(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void forKings(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void forBishops(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void forRooks(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void forQueens(Move::Buffer&, const Board::Type&);

    void forBoard(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void forBoard(Move::Buffer&, const Board::Type&);
}

#endif /* GENERATOR_HPP */
