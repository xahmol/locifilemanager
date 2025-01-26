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

int clock_settime (clockid_t clock_id, const struct timespec* tp)
// Function to set the time of a clock
// Parameters:   clock_id – The clock to set the time of.
//               tp – Pointer to a timespec structure containing the time to set.
// Returns:      0 on success. -1 on error.
// A regs:       return, clock_id, tp
// Errno:        EINVAL, EPERM, EFAULT, EINTR, EIO, ENOSYS
{
    mia_set_ax (clock_id);
    mia_push_long (tp->tv_nsec);
    mia_push_long (tp->tv_sec);
    return mia_call_int_errno (MIA_OP_CLOCK_SETTIME);
}
