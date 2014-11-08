#ifndef SILENTS_HPP
#define SILENTS_HPP

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Silents {
    template <Color::Type COLOR> void phase(Move::Buffer&, const Board::Type&);
}

#endif /* SILENTS_HPP */
