// LOCI API CC65 library
//
// getcwd_loci function
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2025

#include "loci.h"
#include "osdklib.h"

void __fastcall__ getcwd_loci(char *buf, unsigned char len)
// Function to copy CWD from LOCI to RAM buffer
// Input:
//  -   buffer: destination in RAM
//  =   len: max length (exluding trailing zero)
{
    unsigned char val;
    unsigned char count = 0;
    unsigned address = COPYBUF_XRAM_ADDR;

    getcwd_xram(address, 255);

    while (val = xram_peek(address++), val != 0 && count < len)
    {
        count++;
        poke(buf++, val);
    }

    if (len < 255)
    {
        buf[len] = 0;
    }
    else
    {
        buf[255] = 0;
    }
}