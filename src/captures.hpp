#ifndef CAPTURES_HPP
#define CAPTURES_HPP

#include <string>

#include "core.hpp"

#include "boards.hpp"
#include "moves.hpp"

namespace Captures {
    void initTables();

    template <Color::Type COLOR> void pawns(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void knights(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void kings(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void bishops(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void rooks(Move::Buffer&, const Board::Type&);
    template <Color::Type COLOR> void queens(Move::Buffer&, const Board::Type&);

    template <Color::Type COLOR> void board(Move::Buffer&, const Board::Type&);
}

#endif /* CAPTURES_HPP */
