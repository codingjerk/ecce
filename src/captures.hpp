#ifndef CAPTURES_HPP
#define CAPTURES_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Captures {
    template <Color::Type COLOR> void pawns(Move::Buffer&, const Board::Type&, Coord::Type lastMoved);
    template <Color::Type COLOR> void knights(Move::Buffer&, const Board::Type&, Coord::Type lastMoved);
    template <Color::Type COLOR> void kings(Move::Buffer&, const Board::Type&, Coord::Type lastMoved);
    template <Color::Type COLOR> void bishops(Move::Buffer&, const Board::Type&, Coord::Type lastMoved);
    template <Color::Type COLOR> void rooks(Move::Buffer&, const Board::Type&, Coord::Type lastMoved);
    template <Color::Type COLOR> void queens(Move::Buffer&, const Board::Type&, Coord::Type lastMoved);

    template <Color::Type COLOR> void phase(Move::Buffer&, const Board::Type&);
}

#endif /* CAPTURES_HPP */
