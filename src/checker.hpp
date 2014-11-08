#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "core.hpp"
#include "boards.hpp"

namespace Checker {
    template <Color::Type WHO> Boolspeed isCheck(const Board::Type&);
    template <Color::Type WHO> Boolspeed isAttacked(const Board::Type&, const Coord::Type);
}

#endif /* CHECKER_HPP */
