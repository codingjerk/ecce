#ifndef EVALUATION_HPP
#define EVALUATION_HPP

#include "core.hpp"

#include "colors.hpp"
#include "boards.hpp"
#include "scores.hpp"

namespace Eval {
    template <Color::Type COLOR>
    Score::Type material(Board::Type&);
}

#endif /* EVALUATION_HPP */
