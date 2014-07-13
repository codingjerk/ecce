#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Generator {
    const UNumspeed BUFFER_SIZE = 256;
    using MoveBuffer = Move::Type[BUFFER_SIZE];

    void initTables();

    template <Color::Type COLOR> void forPawns(MoveBuffer&, const Board::Type&);
    template <Color::Type COLOR> void forKnights(MoveBuffer&, const Board::Type&);
    template <Color::Type COLOR> void forKings(MoveBuffer&, const Board::Type&);
    template <Color::Type COLOR> void forBishops(MoveBuffer&, const Board::Type&);
    template <Color::Type COLOR> void forRooks(MoveBuffer&, const Board::Type&);
    template <Color::Type COLOR> void forQueens(MoveBuffer&, const Board::Type&);

    void forBoard(MoveBuffer&, const Board::Type&);
    template <Color::Type COLOR> void forBoard(MoveBuffer&, const Board::Type&);
}

#endif /* GENERATOR_HPP */
