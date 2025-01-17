// LOCI API CC65 library
//
// File operation functions
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2025

#include "loci.h"
#include "osdklib.h"
#include <fcntl.h>
#include <conio.h>

// Progress tracking string
const char progressBar[4] = {48, 53, 93, 95};

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

int __fastcall__ file_exists(const char *file)
// Function to check if a file exists
// Input:   file - filename
// Output:  1 if file exists, 0 if not
{
    int fd = open(file, O_RDONLY | O_EXCL);
    if (fd < 0)
    {
        return 0;
    }
    close(fd);
    return 1;
}

int file_copy(const char *dst, const char *src, unsigned char prog, unsigned char progx, unsigned char progy, unsigned char progl)
// Function to copy a file
// Input:   dst - destination filename
//          src - source filename
//          prog - enable progress indicator: 0 = no, 1 = yes
//          progx - progress indicator x screen position
//          progy - progress indicator y screen position
//          progl - progress indicator length
// Output:  error code
{
    int fd_dst;
    int fd_src;
    int len;
    unsigned char cnt = 0;

    fd_src = open(src, O_RDONLY | O_EXCL);
    if (fd_src < 0)
        return fd_src;
    fd_dst = open(dst, O_WRONLY | O_CREAT);

    if (fd_dst < 0)
    {
        close(fd_src);
        return fd_dst;
    }

    // Prepare progress bar
    if (prog)
    {

        gotoxy(progx, progy);
        cputc(A_ALT);
        cclear(progl);
    }

    do
    {
        if (prog)
        {
            // print progress counter
            if ((cnt >> 2) > progl - 2)
            {
                cnt = 0;
                gotoxy(progx + 1, progy);
                cclear(progl-1);
            }
            else
            {
                gotoxy(progx + 1 + (cnt >> 2), progy);
                cputc(progressBar[cnt & 3]);
                ++cnt;
            }
        }

        len = read_xram(COPYBUF_XRAM_ADDR, COPYBUF_XRAM_SIZE, fd_src);
        write_xram(COPYBUF_XRAM_ADDR, len, fd_dst);
    } while (len == COPYBUF_XRAM_SIZE);
    close(fd_src);
    close(fd_dst);

    if (prog)
    {
        gotoxy(progx, progy);
        cclear(progl);
    }
    return 0;
}
