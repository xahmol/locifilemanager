// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2025

#include <loci.h>
#include <unistd.h>

off_t __fastcall__ lseek(int fd, off_t offset, int whence)
// Function to move the read/write pointer.
// This is implemented internally with an argument order to take advantage of short stacking the offset.
// Parameters:  offset – How far you wish to seek.
//              whence – From whence you wish to seek.
//              fildes – File descriptor from open().
// Returns:     Read/write position. -1 on error. If this value would be too large for a long, the returned value will be 0x7FFFFFFF.
// A regs:      fildes
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_INVALID_OBJECT, FR_TIMEOUT
// Whence:      SEEK_SET = 2    The start of the file plus offset bytes.
//              SEEK_CUR = 0    The current location plus offset bytes.
//              SEEK_END = 1    The size of the file plus offset bytes.
{
    /* Modified argument order for short stacking offset */
    mia_push_long(offset);
    mia_push_char(whence);
    mia_set_ax(fd);
    return mia_call_long_errno(MIA_OP_LSEEK);
}
