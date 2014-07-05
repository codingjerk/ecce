#ifndef CORE_HPP
#define CORE_HPP

// --- Arch-depended types ---
using Numspeed = signed long long;
using UNumspeed = unsigned long long;

using Numsize = signed char;
using UNumsize = unsigned char;

using Nummax = signed long long;
using UNummax = unsigned long long;

#define makeNumspeed(num) num##ll
#define makeUNumspeed(num) num##ull

#define makeNumsize(num) num
#define makeUNumsize(num) num

#define makeNummax(num) num##ll
#define makeUNummax(num) num##ull

#endif /* CORE_HPP */
