#ifndef RAND_HPP
#define RAND_HPP

#include <cstdlib>

UNum64 rand64() {
    UNum64 r30 = RAND_MAX*rand()+rand();
    UNum64 s30 = RAND_MAX*rand()+rand();
    UNum64 t4  = rand() & 0xf;

    return (r30 << 34) + (s30 << 4) + t4;
}

#endif /* RAND_HPP */
