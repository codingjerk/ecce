#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <iostream>
#include <sstream>

#include "hash.hpp"

namespace Options {
    void show() {
        std::cout << "option name Hash type spin default 50 min 1 max 4096\n";
    }

    void set(const std::string &name, const std::string &value) {
        if (name == "Hash") {
            Hash::setTableSizeInMb(toNum(value));
        } else {
            std::cout << "Unsuported option, sory.\n";
        }
    }
}

#endif /* OPTIONS_HPP */
