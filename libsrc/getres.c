// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2025

#include <loci.h>
#include <time.h>

extern int __clock_gettimespec(struct timespec *ts, unsigned char op);

int clock_getres(clockid_t clock_id, struct timespec *res)
// Function to copy the clock resolution to res.
// Parameters: clock_id – 0 for CLOCK_REALTIME.
// Returns:    0 on success. -1 on error.
// A regs:     return, clock_id
// Errno:      EINVAL
{
    mia_set_ax(clock_id);
    return __clock_gettimespec(res, MIA_OP_CLOCK_GETRES);
}
