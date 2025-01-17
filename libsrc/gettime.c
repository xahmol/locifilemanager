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

int clock_gettime(clockid_t clock_id, struct timespec *tp)
// Function to copy the current time to tp.
// Parameters:  clock_id – 0 for CLOCK_REALTIME.
// Returns:     0 on success. -1 on error.
// A regs:      return, clock_id
// Errno:       EINVAL, EUNKNOWN
{
    (void)clock_id;
    /* time.s doesn't set the stack value for clock_id (bug?) */
    mia_set_ax(CLOCK_REALTIME);
    return __clock_gettimespec(tp, MIA_OP_CLOCK_GETTIME);
}
