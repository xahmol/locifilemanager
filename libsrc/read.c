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

int __fastcall__ read(int fildes, void *buf, unsigned count)
// Function to read count bytes from a file to a buffer.
// Parameters:  fildes – File descriptor from open().
//              buf – Destination for the returned data.
//              count – Quantity of bytes to read.
// Returns:     On success, number of bytes read is returned. On error, -1 is returned.
// A regs:      fildes
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_DENIED, FR_INVALID_OBJECT, FR_TIMEOUT
{
    int total = 0;
    while (count)
    {
        unsigned blockcount = (count > 256) ? 256 : count;
        int bytes_read = read_xstack(&((char *)buf)[total], blockcount, fildes);
        if (bytes_read < 0)
        {
            return bytes_read;
        }
        total += bytes_read;
        count -= bytes_read;
        if (bytes_read < blockcount)
        {
            break;
        }
    }
    return total;
}
