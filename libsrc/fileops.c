// LOCI API CC65 library
//
// File operation functions
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024

#include "loci.h"
#include "osdklib.h"
#include <fcntl.h>
#include <conio.h>

// Progress tracking string
const char progress_str[] = ".oOo";

int __fastcall__ file_save(const char *file, const void *src, unsigned int count)
// Function to save a file
// Input:   file - filename
//          src - source buffer
//          count - number of bytes to write
// Output:  error code
{
    int fd = open(file, O_WRONLY | O_CREAT);
    int error;

    if (fd < 0)
    {
        return fd;
    }

    error = write(fd, src, count);
    close(fd);
    return error;
}

int __fastcall__ file_load(const char *file, void *dst, unsigned int count)
// Function to load a file
// Input:   file - filename
//          dst - destination buffer
//          count - number of bytes to read
// Output:  error code
{
    int fd = open(file, O_RDONLY | O_EXCL);
    int error;

    if (fd < 0)
    {
        return fd;
    }

    error = read(fd, dst, count);
    close(fd);
    return error;
}

int __fastcall__ file_copy(const char *dst, const char *src, unsigned char prog, unsigned char ypos)
// Function to copy a file
// Input:   dst - destination filename
//          src - source filename
//          prog - enable progress indicator: 0 = no, 1 = yes
//          progx - progress indicator x screen position
//          progy - progress indicator y screen position
// Output:  error code
{
    int fd_dst;
    int fd_src;
    int len;
    unsigned char cnt;

    fd_src = open(src, O_RDONLY | O_EXCL);
    if (fd_src < 0)
        return fd_src;
    fd_dst = open(dst, O_WRONLY | O_CREAT);

    if (fd_dst < 0)
    {
        close(fd_src);
        return fd_dst;
    }

    do
    {
        len = read_xram(FM_XRAM_ADDR, FM_XRAM_SIZE, fd_src);
        write_xram(FM_XRAM_ADDR, len, fd_dst);
        if (prog)
        {
            gotoxy(38, ypos);
            cputc(A_FWGREEN);
            cputc(progress_str[0x03 & cnt++]);
        }
    } while (len == FM_XRAM_SIZE);
    close(fd_src);
    close(fd_dst);
    return 0;
}
