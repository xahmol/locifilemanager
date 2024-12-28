// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <loci.h>

int __fastcall__ mount(int drive, register const char *path, register const char *filename)
// Function to mount a file to a drive
// Input:   drive - drive number
//          path - path to the file
//          filename - filename
// Output:  error code
{
    unsigned i;
    mia_set_ax(drive);
    for (i = strlen(filename); i;)
    {
        mia_push_char(((char *)filename)[--i]);
    }
    mia_push_char('/');
    for (i = strlen(path); i;)
    {
        mia_push_char(((char *)path)[--i]);
    }
    return mia_call_int_errno(MIA_OP_MOUNT);
}

int __fastcall__ umount(int drive)
// Function to unmount a drive
// Input:   drive - drive number
// Output:  error code
{
    mia_set_ax(drive);
    return mia_call_int_errno(MIA_OP_UMOUNT);
}
