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

int clock_gettimezone(clockid_t clock_id, struct _timezone *tz)
// Function to get the current timezone
// Input:   clock_id - clock id
//          tz - timezone structure
// Output:  error code
{
    int ax;
    mia_set_ax(clock_id);
    ax = mia_call_int_errno(MIA_OP_CLOCK_GETTIMEZONE);
    if (ax >= 0)
    {
        char i;
        for (i = 0; i < sizeof(struct _timezone); i++)
        {
            ((char *)tz)[i] = mia_pop_char();
        }
    }
    return ax;
}
