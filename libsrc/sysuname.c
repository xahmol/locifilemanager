// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024

#include <loci.h>
#include <errno.h>
#include <sys/utsname.h>

int __fastcall__ _sysuname(struct utsname *buf)
// Get system information.
// Parameters:  buf – Pointer to a utsname structure to store the information.
// Returns:     0 on success. -1 on error.
{
    unsigned i;
    int ret;

    ret = mia_call_int_errno(MIA_OP_UNAME);

    for (i = 0; i < sizeof(struct utsname); i++)
    {
        ((char *)buf)[i] = mia_pop_char();
    }
    return ret;
}