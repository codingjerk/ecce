#ifndef MOVER_HPP
#define MOVER_HPP

#include "core.hpp"

#include "moves.hpp"
#include "boards.hpp"

namespace Move {
    Boolspeed make(Type, Board::Type&);
    void unmake(Type, Board::Type&);
}

#endif /* MOVER_HPP */
