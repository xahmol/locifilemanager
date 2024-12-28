
// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024

#include <loci.h>

int __fastcall__ stdin_opt(unsigned long ctrl_bits, unsigned char str_length)
// Additional options for the STDIN line editor. Set the str_length to your buffer size - 1 to make gets() safe.
// This can also guarantee no split lines when using fgets() on STDIN.
// Parameters:  ctrl_bits – Bitmap of ASCII 0-31 defines which CTRL characters can abort an input.
//                          When CTRL key is pressed, any typed input remains on the screen but the applicaion receives a line
//                          containing only the CTRL character. e.g. CTRL-C + newline.
//              str_length – 0-255 default 254. The input line editor won’t allow user input greater than this length.
// A regs:      return, str_length
// Returns:     0 on success
// Errno:       will not fail
{
    mia_push_long(ctrl_bits);
    mia_set_a(str_length);
    return mia_call_int_errno(MIA_OP_STDIN_OPT);
}
