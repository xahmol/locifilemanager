// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025

#include <loci.h>

long __fastcall__ lrand(void)
// Function to generates a random number starting with entropy on the MIA.
// This is suitable for seeding a RNG or general use.
// The 16-bit rand() in the CC65 library can be seeded with this by calling its non-standard _randomize() function.
// Returns: Chaos. 0x0 <= x <= 0x7FFFFFFF
// Errno:   will not fail
{
    return mia_call_long(MIA_OP_LRAND);
}
