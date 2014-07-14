#include "evaluation.hpp"

using namespace Eval;

template <Color::Type COLOR>
Score::Type Eval::material(Board::Type&) {
    return 0;
}

template Score::Type Eval::material<White>(Board::Type&);
template Score::Type Eval::material<Black>(Board::Type&);
