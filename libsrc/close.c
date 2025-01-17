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

int __fastcall__ close(int fd)
// Function to close a file
// Release the file descriptor. File descriptor will rejoin the pool available for use by open().
// Parameters:  	fildes – File descriptor from open().
// Returns:         0 on success. -1 on error.
// A regs:          return, fildes
// Errno    	    EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_INVALID_OBJECT, FR_TIMEOUT
{
    mia_set_ax(fd);
    return mia_call_int_errno(MIA_OP_CLOSE);
}
