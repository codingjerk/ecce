#ifndef CAPTURES_HPP
#define CAPTURES_HPP

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Captures {
    template <Color::Type COLOR> void phase(Move::Buffer&, const Board::Type&);
}

#endif /* CAPTURES_HPP */
