#ifndef USELESSES_HPP
#define USELESSES_HPP

// @TODO: remove useless includes from all files
#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Uselesses {
    template <Color::Type COLOR> void phase(Move::Buffer&, const Board::Type&);
}

#endif /* USELESSES_HPP */
