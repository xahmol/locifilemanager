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

int __cdecl__ open(const char *name, int flags, ...)
// Function to open a file. Create a connection between a file and a file descriptor.
// Parameters:  path – Pathname to a file.
//              flag – options
//  Returns:    File descriptor. -1 on error.
//  A regs      return, oflag
//  Errno:      EINVAL, EMFILE, FR_DISK_ERR, FR_INT_ERR, FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME, FR_DENIED,
//              FR_EXIST, FR_INVALID_OBJECT, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM, FR_TIMEOUT,
//              FR_LOCKED, FR_NOT_ENOUGH_CORE, FR_TOO_MANY_OPEN_FILES
//  Options 	O_RDONLY 0x01   Open for reading only.
//              O_WRONLY 0x02   Open for writing only.
//              O_RDWR 0x03     Open for reading and writing.
//              O_CREAT 0x10    Create the file if it does not exist.
//              O_TRUNC 0x20    Truncate the file length to 0 after opening.
//              O_APPEND 0x40   Read/write pointer is set end of the file.
//              O_EXCL 0x80     If O_CREAT and O_EXCL are set, fail if the file exists.
{
    size_t namelen = strlen(name);
    if (namelen > 255)
    {
        return _mappederrno(EINVAL);
    }
    while (namelen)
    {
        mia_push_char(name[--namelen]);
    }
    mia_set_ax(flags);
    return mia_call_int_errno(MIA_OP_OPEN);
}
