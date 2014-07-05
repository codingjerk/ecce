#include "cstf.hpp"

#include "coords.hpp"
#include "moves.hpp"

int main(int, char**) {
    SECTION(Framework);
    CHECK(true);
    EXCEPTION(throw 1);

    SECTION(Coords);
    CHECK(Coord::fromString("a1") == 0);
    CHECK(Coord::show(0ull) == "a1");
    CHECK(Coord::show(Coord::fromString("h8")) == "h8");

    SECTION(Moves);
    CHECK(Move::fromString("a1a1") == 0);
    CHECK(Move::show(0) == "a1a1");
    CHECK(Move::show(Move::fromString("a8h1")) == "a8h1");

    RESULTS;

    return 0;
}
