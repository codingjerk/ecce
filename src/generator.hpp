#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"

namespace Generator {
    using MoveBuffer = Move::Type [256];

    void initTables();

    void forKnights(MoveBuffer&, const Board::Type&);

    template <Color::Type COLOR>
    void forKnights(MoveBuffer&, const Board::Type&);
}

#endif /* GENERATOR_HPP */
