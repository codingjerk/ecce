#ifndef RECAPTURES_HPP
#define RECAPTURES_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Recaptures {
    template <Color::Type COLOR> void phase(Move::Buffer&, const Board::Type&);
}   

#endif /* RECAPTURES_HPP */
