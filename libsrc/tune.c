// LOCI API CC65 library
//
// Timing tuning functions
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025

#include <loci.h>

int __fastcall__ tune_tmap(unsigned char delay)
// Function to tune the TMAP timing
// Input:   delay - delay value
// Output:  error code
{
    mia_set_ax(delay);
    return mia_call_int_errno(MIA_OP_TUNE_TMAP);
}

int __fastcall__ tune_tior(unsigned char delay)
// Function to tune the TIOR timing
// Input:   delay - delay value
// Output:  error code
{
    mia_set_ax(delay);
    return mia_call_int_errno(MIA_OP_TUNE_TIOR);
}

int __fastcall__ tune_tiow(unsigned char delay)
// Function to tune the TIOW timing
// Input:   delay - delay value
// Output:  error code
{
    mia_set_ax(delay);
    return mia_call_int_errno(MIA_OP_TUNE_TIOW);
}

int __fastcall__ tune_tiod(unsigned char delay)
// Function to tune the TIOD timing
// Input:   delay - delay value
// Output:  error code
{
    mia_set_ax(delay);
    return mia_call_int_errno(MIA_OP_TUNE_TIOD);
}

int __fastcall__ tune_tadr(unsigned char delay)
// Function to tune the TADR timing
// Input:   delay - delay value
// Output:  error code
{
    mia_set_ax(delay);
    return mia_call_int_errno(MIA_OP_TUNE_TADR);
}

void __fastcall__ tune_scan_enable(void)
// Function to enable the scan tuning
{
    mia_call_void(MIA_OP_TUNE_SCAN);
}