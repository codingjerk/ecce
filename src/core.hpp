#ifndef CORE_HPP
#define CORE_HPP

// --- Main definitions ---
#define DEBUGMODE

// --- Arch-depended types ---
using Num64 = signed long long;
using UNum64 = unsigned long long;

using Numspeed = signed long long;
using UNumspeed = unsigned long long;

using Numsize = signed char;
using UNumsize = unsigned char;

using Nummax = signed long long;
using UNummax = unsigned long long;

#define makeNum64(num) num##ll
#define makeUNum64(num) num##ull

#define makeNumspeed(num) num##ll
#define makeUNumspeed(num) num##ull

#define makeNumsize(num) num
#define makeUNumsize(num) num

#define makeNummax(num) num##ll
#define makeUNummax(num) num##ull

// --- Usefull macroses ---
#define SHOW(var) std::cout << #var << " = " << var << "\n";

#endif /* CORE_HPP */
