// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <loci.h>

DIR *__fastcall__ opendir(register const char *name)
// Function to open a directory.
// Parameters:  name – Pointer to a string containing the name of the directory to open.
// Returns:     A pointer to a DIR structure representing the directory stream.
//              NULL if the directory cannot be opened.
// A regs:      name    Pointer to the name of the directory to open.
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_DENIED, FR_INVALID_OBJECT, FR_TIMEOUT
{
    int ret;
    static DIR d;
    size_t namelen = strlen(name);
    while (namelen)
    {
        mia_push_char(((char *)name)[--namelen]);
    }
    ret = mia_call_int_errno(MIA_OP_OPENDIR);
    d.fd = ret;

    // Copy string buffer overflow safe and ensure trailing zero
    strncpy(d.name, name, sizeof(d.name));
    d.name[256] = 0;

    d.off = 0;
    return &d;
}
