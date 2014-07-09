#ifndef MOVER_HPP
#define MOVER_HPP

#include "core.hpp"

#include "moves.hpp"
#include "boards.hpp"

namespace Move {
    UNumspeed make(Type, Board::Type&);
    void unmake(Type, Board::Type&);

    const UNumspeed success = 1;
    const UNumspeed fail = 0;
}

#endif /* MOVER_HPP */
