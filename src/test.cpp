#include "cstf.hpp"
#include "moves.hpp"

int main(int, char**) {
    SECTION(Framework);
    CHECK(2 == 2);

    RESULTS;

    return 0;
}
