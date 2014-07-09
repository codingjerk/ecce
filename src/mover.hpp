#ifndef MOVER_HPP
#define MOVER_HPP

#include "core.hpp"

#include "moves.hpp"

namespace Restore {
    using Type = UNumspeed;
    void restore(Type);
}

namespace Move {
    Restore::Type make(Type);
}

#endif /* MOVER_HPP */
