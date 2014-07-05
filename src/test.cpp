#include "cstf.hpp"

#include "coords.hpp"

int main(int, char**) {
    SECTION(Framework);
    CHECK(true);
    EXCEPTION(throw 1);

    SECTION(Coord);
    CHECK(Coord::fromString("a1") == 0);
    CHECK(Coord::show(0ull) == "a1");
    CHECK(Coord::show(Coord::fromString("h8")) == "h8");

    RESULTS;

    return 0;
}
