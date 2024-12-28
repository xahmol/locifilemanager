// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024
//
// Based on code by Groepaz.
// 2012-05-30, Ullrich von Bassewitz
// 2021-02-15, Greg King
// 2024-05-27, Sodiumlightbaby

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <loci.h>

struct dirent *__fastcall__ readdir(register DIR *dir)
// Function to read the next directory entry.
// Parameters:  dir – Pointer to a DIR structure.
// Returns:     A pointer to a dirent structure representing the next directory entry.
//              NULL if the end of the directory is reached or an error occurs.
// A regs:      dir
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_DENIED, FR_INVALID_OBJECT, FR_TIMEOUT
{
    unsigned i;
    char d[sizeof(struct dirent)];

    mia_set_ax(dir->fd);
    mia_call_int_errno(MIA_OP_READDIR);

    for (i = 0; i < sizeof(struct dirent); i++)
    {
        d[i] = mia_pop_char();
    }
    return (struct dirent *)&d;
}
