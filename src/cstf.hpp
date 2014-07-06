#ifndef CSTF_HPP
#define CSTF_HPP

#include <iostream>
#include <iomanip>
#include <mutex>

#include "core.hpp"

extern std::mutex outMutex;

extern UNummax checkCounter;
extern UNummax failedCounter;
extern UNummax passedCounter;

extern bool firstSection;

extern UNummax maxTestInfoSize;

std::string firsts(std::string);

// --- TDD tests ---

#define SECTION(name) { \
    outMutex.lock(); \
    if (!firstSection) std::cout << "\n"; else firstSection = false;\
    std::cout << "---" << #name << " Testing---\n"; \
    outMutex.unlock(); \
}

#define CHECK(expr) { \
    outMutex.lock(); \
    using namespace std; \
    checkCounter++; \
    if (expr) { \
        cout << "    Passed "; \
        passedCounter++; \
    } else { \
        cout << "!!! Failed "; \
        failedCounter++; \
    } \
    cout << "Test #" \
        << setfill('0') << setw(4) << (checkCounter) \
        << " (" << firsts(#expr) << ")\n"; \
    outMutex.unlock(); \
}

#define EXCEPTION(expr) { \
    outMutex.lock(); \
    using namespace std; \
    checkCounter++; \
    if (checkCounter == 1) { \
        cout << "Starting tests...\n"; \
    } else {}; \
    try { \
        expr; \
        cout << "!!! Failed "; \
        failedCounter++; \
    } catch (...) { \
        cout << "    Passed "; \
        passedCounter++; \
    } \
    cout << "Exception Test #" \
        << setfill('0') << setw(4) << (checkCounter) \
		<< " (" << firsts(#expr) << ")\n"; \
    outMutex.unlock(); \
}

#define RESULTS { \
    using namespace std; \
    SECTION(Test results); \
    cout << "    Passed " << passedCounter << "/" << checkCounter << ".\n"; \
    if (failedCounter == 0) {cout << "    All tests passed.\n";} \
    else {cout << "    Failed " << failedCounter << " tests.\n";} \
    return failedCounter; \
}

// --- Runtime asserts (debug mode) ---
#ifdef DEBUGMODE
    #define ASSERT(expr) {\
        if (!(expr)) std::cout << "!!! Assert was failed (" << #expr << ")\n";\
    }
#else
    #define ASSERT(expr)
#endif

#endif /* CSTF_HPP */
