#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef OSLINUX
#include <sys/time.h>

inline unsigned long GetTickCount()
{
    struct timeval tv;
    if( gettimeofday(&tv, 0) != 0 )
        return 0;
 
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
#endif

#endif /* UTILS_HPP */
