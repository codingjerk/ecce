#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef OSLINUX
#include <sys/time.h>
#include <unistd.h>

inline unsigned long GetTickCount()
{
    struct timeval tv;
    if( gettimeofday(&tv, 0) != 0 )
        return 0;
 
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

#define min(a,b) ((a)<(b))?(a):(b)
#endif

#ifndef OSLINUX
    #include <Windows.h>
#endif

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

#endif /* UTILS_HPP */
