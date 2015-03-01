#include "cstf.hpp"

std::mutex outMutex;

UNummax checkCounter = 0;
UNummax failedCounter = 0;
UNummax passedCounter = 0;

bool firstSection = true;

UNummax maxTestInfoSize = 35;

std::string firsts(const std::string &old) {
    if (old.size() <= maxTestInfoSize) {
        return old;
    } else {
        return old.substr(0,maxTestInfoSize).append("...");
    }
}
