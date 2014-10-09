#ifndef INTERUPTERS_HPP
#define INTERUPTERS_HPP

#include "core.hpp"
#include "utils.hpp"

namespace Search {
	typedef bool (*Interupter)();  

    bool stopSearch;
	UNumspeed totalNodes;
	UNumspeed endTime;

    inline bool stopInterupter() {
        if(totalNodes % 1024 == 0 && isInputAvailable()) {
            std::string input;
            std::getline(std::cin, input);
            return (input == "stop");
        }

        return false;
    }

    inline bool timeInterupter() {
        return GetTickCount() >= endTime || stopInterupter();
    }
}

#endif /* INTERUPTERS_HPP */
