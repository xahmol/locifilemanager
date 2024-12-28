// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024

#include <loci.h>

int __fastcall__ read_xstack(void *buf, unsigned count, int fildes)
// Function to read count bytes from a file to xstack.
// Parameters:  buf – Destination for the returned data.
//              count – Quantity of bytes to read. 0x7FFF max.
//              fildes – File descriptor from open().
// Returns:     On success, number of bytes read is returned. On error, -1 is returned.
// A regs:      fildes
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_DENIED, FR_INVALID_OBJECT, FR_TIMEOUT
{
    int i, ax;
    mia_push_int(count);
    mia_set_ax(fildes);
    ax = mia_call_int_errno(MIA_OP_READ_XSTACK);
    for (i = 0; i < ax; i++)
    {
        ((char *)buf)[i] = mia_pop_char();
    }
    return ax;
}
