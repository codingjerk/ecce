#ifndef SILENTS_HPP
#define SILENTS_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Silents {
    template <Color::Type COLOR> void pawns(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void knights(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void kings(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void bishops(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void rooks(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void queens(Move::Buffer&, const Board::Type&);

    template <Color::Type COLOR> void phase(Move::Buffer&, const Board::Type&);
}

#endif /* SILENTS_HPP */
