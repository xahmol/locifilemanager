// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2025

#include <loci.h>
#include <errno.h>
#include <string.h>

int __fastcall__ _sysmkdir (const char* name)
// Create a new directory.
// Parameters: name – The name of the directory to create.
// Returns:    0 on success. -1 on error.
{
    int ret;
    size_t namelen = strlen(name);
    while(namelen) {
        mia_push_char (((char*)name)[--namelen]);
    }
    return mia_call_int_errno (MIA_OP_MKDIR);

    return ret;
}