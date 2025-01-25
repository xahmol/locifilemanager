// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025

#include <loci.h>
#include <unistd.h>

int __fastcall__ write(int fildes, const void *buf, unsigned count)
// Function to write count bytes from a buffer to a file.
// Parameters:  fildes – File descriptor from open().
//              buf – Source for the data to write.
//              count – Quantity of bytes to write.
// Returns:     On success, number of bytes written is returned. On error, -1 is returned.
// A regs:      fildes
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_DENIED, FR_INVALID_OBJECT, FR_TIMEOUT
{
    int ax, total = 0;
    while (count)
    {
        int blockcount = (count > 256) ? 256 : count;
        ax = write_xstack(&((char *)buf)[total], blockcount, fildes);
        if (ax < 0)
        {
            return ax;
        }
        total += ax;
        count -= ax;
        if (ax < blockcount)
        {
            break;
        }
    }
    return total;
}
