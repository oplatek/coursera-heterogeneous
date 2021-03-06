// Author:   Ondrej Platek, Copyright 2012, code is without any warranty!
// Created:  10:44:31 03/12/2012
// Modified: 10:44:31 03/12/2012


/*

The documentation from 
http://tdistler.com/2010/06/27/high-performance-timing-on-linux-windows

Linux Clock

POSIX.1b defines realtime clock methods that you’ll find on most *NIX systems (the full spec can be viewed HERE). Specifically, you want to use clock_getres() and clock_gettime(). clock_getres() returns the resolution (frequency) of the clock, and clock_gettime() returns the current value of the clock. Most systems implement the CLOCK_MONOTONIC type, which provides a frequency-stable, monotonically-increasing counter. The resolution of CLOCK_MONOTONIC is high on the 2.6 kernel, in my experience. I recommend using this clock when building a high-performance timing solutions on Linux.

The methods are defined in time.h, and you need to link against librt (pass ‘-lrt’ to gcc). The prototypes for the functions are:

int clock_getres(clockid_t clock_id, struct timespec *res);
int clock_gettime(clockid_t clock_id, struct timespec *tp);

A detailed description of these methods can be found HERE.

I also suggest looking at clock_nanosleep(), but that’s a separate topic.

*/

#include <time.h>
#include <stdio.h>
#include "cudatimer.h"

// Namespace because windows.h causes errors
namespace CudaTimerNS
{
    CudaTimer::CudaTimer()
    {
        unsigned long freq;
        struct timespec tp;
        // TODO is this the correct type 
        if ( clock_getres(CLOCK_REALTIME, &tp) != 0 )
            printf("Cannot access timer\n");
        else
            freq_ = 1.0 / tp.tv_nsec;
    }

    unsigned long CudaTimer::getTime()
    {
        unsigned long time = 0;
        struct timespec tp;
        // TODO change to correct type
        if(clock_gettime(CLOCK_REALTIME, &tp))
            printf("Cannot get timer value\n");
        else
            time = tp.tv_nsec;
        return time;
    }

}
