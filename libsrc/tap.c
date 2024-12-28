// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024

#include "loci.h"

long tap_seek(long pos)
// Tape: search for file at position
// Input:   pos - position to seek to
// Output:  position file is found
{
    mia_set_axsreg(pos);
    return mia_call_long_errno(MIA_OP_TAP_SEEK);
}

long tap_tell(void)
// Tape: get tape position
// Output:  position
{
    return mia_call_long(MIA_OP_TAP_TELL);
}

long tap_read_header(tap_header_t *header)
// Tape: Read next header from current postion of tap file
// Input:   header - pointer to header structure
// Output:  position
{
    unsigned char i;
    unsigned char *h = (unsigned char *)header;
    long axsreg;
    axsreg = mia_call_long_errno(MIA_OP_TAP_HDR);
    for (i = 0; i < sizeof(tap_header_t); i++)
    {
        h[i] = mia_pop_char();
    }
    return axsreg;
}
