#include "cstf.hpp"
#include "moves.hpp"

int main(int, char**) {
    SECTION(Framework);
    CHECK(true);
    EXCEPTION(throw 1);

    RESULTS;

    return 0;
}
