// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025

#include <loci.h>

int __fastcall__ read_xram(unsigned buf, unsigned count, int fildes)
// Function to read count bytes from a file to xram.
// Parameters:  buf – Destination for the returned data.
//              count – Quantity of bytes to read. 0x7FFF max.
//              fildes – File descriptor from open().
// Returns:     On success, number of bytes read is returned. On error, -1 is returned.
// A regs:      fildes
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_DENIED, FR_INVALID_OBJECT, FR_TIMEOUT
{
    mia_push_int(buf);
    mia_push_int(count);
    mia_set_ax(fildes);
    return mia_call_int_errno(MIA_OP_READ_XRAM);
}
