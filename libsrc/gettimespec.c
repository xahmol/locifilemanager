// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025

#include <loci.h>
#include <time.h>

int __clock_gettimespec(struct timespec *ts, unsigned char op)
/* Internal method shared by clock_getres and clock_gettime. */
{
    int ax = mia_call_int_errno(op);
    if (ax >= 0)
    {
        ts->tv_sec = mia_pop_long();
        ts->tv_nsec = mia_pop_long();
    }
    return ax;
}
