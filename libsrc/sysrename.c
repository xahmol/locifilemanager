// LOCI API CC65 library
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025

#include <loci.h>
#include <errno.h>
#include <string.h>

unsigned char __fastcall__ _sysrename (const char* oldpath, const char* newpath)
// Renames and/or moves a file or directory.
// Parameters:  oldname – Existing file or directory name to rename.
//              newname – New object name.
// Returns:     0 on success. -1 on error.
// Errno:       EINVAL, FR_DISK_ERR, FR_INT_ERR, FR_NOT_READY, FR_NO_FILE, FR_NO_PATH, FR_INVALID_NAME,
//              FR_EXIST, FR_WRITE_PROTECTED, FR_INVALID_DRIVE, FR_NOT_ENABLED, FR_NO_FILESYSTEM, FR_TIMEOUT,
//              FR_LOCKED, FR_NOT_ENOUGH_CORE
{
    size_t oldpathlen, newpathlen;
    oldpathlen = strlen (oldpath);
    newpathlen = strlen (newpath);
    if (oldpathlen + newpathlen > 254) {
        return _mappederrno (EINVAL);
    }
    while (oldpathlen) {
        mia_push_char (oldpath[--oldpathlen]);
    }
    mia_push_char (0);
    while (newpathlen) {
        mia_push_char (newpath[--newpathlen]);
    }
    return mia_call_int_errno (MIA_OP_RENAME);
}
