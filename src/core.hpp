#ifndef CORE_HPP
#define CORE_HPP

// --- OS ---
// Remove this for compile on windows with visual studio
// #define OSLINUX

#ifdef
#include <sys/time.h>

inline unsigned long GetTickCount()
{
    struct timeval tv;
    if( gettimeofday(&tv, 0) != 0 )
        return 0;
 
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
#endif

// --- Arch-depended types ---
using Boolspeed = unsigned long long;

using Num64 = signed long long;
using UNum64 = unsigned long long;

using Numspeed = signed long long;
using UNumspeed = unsigned long long;

using Numsize = signed char;
using UNumsize = unsigned char;

using Nummax = signed long long;
using UNummax = unsigned long long;

// --- Additional macroses to making values
#define makeBoolspeed(num) num##ull

#define makeNum64(num) num##ll
#define makeUNum64(num) num##ull

#define makeNumspeed(num) num##ll
#define makeUNumspeed(num) num##ull

#define makeNumsize(num) num
#define makeUNumsize(num) num

#define makeNummax(num) num##ll
#define makeUNummax(num) num##ull

// --- Main definitions ---
#define DEBUGMODE
#undef DEBUGMODE

const UNumspeed MAX_DEPTH = makeUNumspeed(300);
const UNumspeed MAX_MOVES = makeUNumspeed(300);

#endif /* CORE_HPP */
