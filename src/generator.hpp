#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"

namespace Generator {
    const UNumspeed BUFFER_SIZE = 1024;
    using MoveBuffer = Move::Type[BUFFER_SIZE];

    void initTables();

    void forKnights(MoveBuffer&, const Board::Type&);

    template <Color::Type COLOR>
    void forKnights(MoveBuffer&, const Board::Type&);

    void forKings(MoveBuffer&, const Board::Type&);

    template <Color::Type COLOR>
    void forKings(MoveBuffer&, const Board::Type&);
}

#endif /* GENERATOR_HPP */
