#ifndef INTERUPTERS_HPP
#define INTERUPTERS_HPP

#include "core.hpp"
#include "utils.hpp"

namespace Search {
    typedef bool (*Interupter)();  

    static bool stopSearch;
    static UNumspeed totalNodes;
    static UNumspeed endTime;

    template <UNumspeed MODULO = 4095>
    inline bool stopInterupter() {
        if((totalNodes & MODULO) == 0 && isInputAvailable()) {
            std::string input;
            std::getline(std::cin, input);
            return (input == "stop");
        }

        return false;
    }

    inline bool timeInterupter() {
        return GetTickCount() >= endTime || stopInterupter<16383>();
    }
}

#endif /* INTERUPTERS_HPP */
