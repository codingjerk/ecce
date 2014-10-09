#ifndef INTERUPTERS_HPP
#define INTERUPTERS_HPP

#include "core.hpp"

#ifdef OSLINUX
    #include <unistd.h>
    #define min(a,b) ((a)<(b))?(a):(b)
#else
    #include <Windows.h>
#endif

namespace Search {
	typedef bool (*Interupter)();  

    bool stopSearch;
	UNumspeed totalNodes;
	UNumspeed endTime;
    
    bool isInputAvailable() {
    #ifdef OSLINUX
        int val;
        fd_set set[1];
        struct timeval time_val[1];

        FD_ZERO(set);
        FD_SET(STDIN_FILENO,set);

        time_val->tv_sec = 0;
        time_val->tv_usec = 0;

        val = select(STDIN_FILENO+1,set,NULL,NULL,time_val);

        return val > 0;
    #else
        static bool init = false, is_pipe;
        static HANDLE stdin_h;
        DWORD val, error;

        if (stdin->_cnt > 0) return true;

        if (!init) {
            init = true;
            stdin_h = GetStdHandle(STD_INPUT_HANDLE);
            is_pipe = !GetConsoleMode(stdin_h, &val); 

            if (!is_pipe) {
                SetConsoleMode(stdin_h, val & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
                FlushConsoleInputBuffer(stdin_h); 
            }
        }

        if (is_pipe) {
            if (!PeekNamedPipe(stdin_h, NULL, 0, NULL, &val, NULL)) {
                return true;
            }
            return val > 0;
        } else {
            GetNumberOfConsoleInputEvents(stdin_h, &val);
            return val > 1;
        }

        return false;
    #endif
    }

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
