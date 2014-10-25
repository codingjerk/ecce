#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <iostream>
#include <sstream>

#include "hash.hpp"

namespace Options {
    void show() {
        std::cout << "option name Hash type spin default 10 min 1 max 2048\n";
    }

    void set(std::string name, std::string value) {
        if (name == "Hash") {
            std::stringstream ss(value);
            UNumspeed size;
            ss >> size;
            Hash::setTableSizeInMb(size);
        } else {
            std::cout << "Unsuported option, sory.\n";
        }
    }
}

#endif /* OPTIONS_HPP */
