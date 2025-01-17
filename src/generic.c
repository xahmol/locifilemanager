/*
LOCI File Manager
File manager for the LOCI mass storage device for Oric Atmos
Written in 2025 by Xander Mol

https://github.com/xahmol/locifilemanager
https://www.idreamtin8bits.com/

For information and documentation on the LOCI mass storage device:
-   LOCI User Manual
    https://github.com/sodiumlb/loci-hardware/wiki/LOCI-User-Manual
-   Sellers of my assembled LOCI device:
    - Raxiss: https://www.raxiss.com/article/id/38-LOCI

Code and resources from others used:
-   LOCI ROM by Sodiumlightbaby, 2024
    https://github.com/sodiumlb/loci-rom

-   CC65 cross compiler:
    https://cc65.github.io/

-   DraBrowse source code for inspiration and text input routine
    DraBrowse (db*) is a simple file browser.
    Originally created 2009 by Sascha Bader.
    Used version adapted by Dirk Jagdmann (doj)
    https://github.com/doj/dracopy

-   lib-ijk-egoist from oricOpenLibrary (for joystick support via Raxiss IJK interface)
    By Raxiss, (c) 2021
    https://github.com/iss000/oricOpenLibrary/tree/main/lib-ijk-egoist

-   forum.defence-force.org: For inspiration and advice while coding.

-   Original windowing system code on Commodore 128 by unknown author.
   
-   Tested using real hardware Oric Atmos plus LOCI

The code can be used freely as long as you retain
a notice describing original source and author.

THE PROGRAMS ARE DISTRIBUTED IN THE HOPE THAT THEY WILL BE USEFUL,
BUT WITHOUT ANY WARRANTY. USE THEM AT YOUR OWN RISK!
*/

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <peekpoke.h>
#include <ctype.h>
#include <atmos.h>
#include "osdklib.h"
#include "loci.h"
#include "defines.h"
#include "generic.h"
#include "menu.h"
#include "dir.h"

// Generic routines
void wait(unsigned int wait_cycles)
// Function to wait for the specified number of cycles
// Input: wait_cycles = numnber of cycles to wait
{

    unsigned int starttime = clock();
    while (clock() - starttime < wait_cycles)
        ;
}

unsigned char getkey(unsigned char joyallowed)
// Function to wait on valid key or joystick press/move
//  Input:  joyallowed (1 if joystick allowed, 0 if not)
//  Output: key value (or joystick converted to key value)
{
    unsigned char key;

    do
    {
        key = 0;
        if (ijk_present && joyallowed)
        {
            ijk_read();
            if (ijk_ljoy & IJK_JOY_FIRE)
            {
                key = CH_ENTER;
            }
            if (ijk_ljoy & IJK_JOY_RIGHT)
            {
                key = CH_CURS_RIGHT;
            }
            if (ijk_ljoy & IJK_JOY_LEFT)
            {
                key = CH_CURS_LEFT;
            }
            if (ijk_ljoy & IJK_JOY_DOWN)
            {
                key = CH_CURS_DOWN;
            }
            if (ijk_ljoy & IJK_JOY_UP)
            {
                key = CH_CURS_UP;
            }
            if (ijk_rjoy & IJK_JOY_FIRE)
            {
                key = CH_ENTER;
            }
            if (ijk_rjoy & IJK_JOY_RIGHT)
            {
                key = CH_CURS_RIGHT;
            }
            if (ijk_rjoy & IJK_JOY_LEFT)
            {
                key = CH_CURS_LEFT;
            }
            if (ijk_rjoy & IJK_JOY_DOWN)
            {
                key = CH_CURS_DOWN;
            }
            if (ijk_rjoy & IJK_JOY_UP)
            {
                key = CH_CURS_UP;
            }
            if (key)
            {
                do
                {
                    ijk_read();
                } while (ijk_ljoy || ijk_rjoy);
                wait(10);
            }
        }
        if (key == 0)
        {
            if (kbhit())
            {
                key = cgetc();
            }
        }
    } while (key == 0);
    return key;
}

void cleararea(unsigned char ypos, unsigned char height)
//  Clear an area of the screen
//  Input: ypos = startline of area
//         height = height of area
{
    unsigned char y;
    for (y = 0; y < height; y++)
    {
        gotoxy(0, ypos + y);
        cputc(A_FWWHITE);
        cputc(A_BGBLACK);
        cclear(38);
    }
}

signed int textInput(unsigned char xpos, unsigned char ypos, unsigned char width, char *str, unsigned char size, unsigned char validation)
{

    /**
     * input/modify a string.
     * based on version DraCopy 1.0e, then modified.
     * Created 2009 by Sascha Bader.
     * @param[in] xpos screen x where input starts.
     * @param[in] ypos screen y where input starts.
     * @param[in,out] str string that is edited, it can have content and must have at least @p size + 1 bytes. Maximum size     if 255 bytes.
     * @param[in] size maximum length of @p str in bytes.
     * @return -1 if input was aborted.
     * @return >= 0 length of edited string @p str.
     */
    // Added input: Validation  =   0   All printable characters allowed
    //                          +   1   Numbers allowed only
    //                          +   2   Also alphabet allowed (lower and uppercase)
    //                          +   4   Also wildcards * and ? allowed
    //                          Add numbers to combine or 0 for no validation
    //              width:      =   width of input viewport, can be less than size

    unsigned char c;
    unsigned char idx = strlen(str);
    unsigned char len;
    unsigned char valid = 0;
    unsigned char offs = 0;

    // Loop for input from keyboard
    while (1)
    {
        // Calculate offset for viewing long string in viewport based on cursor pos
        if (idx + 1 > width)
        {
            offs = idx + 1 - width;
        }
        else
        {
            offs = 0;
        }
        len = strlen(str);

        // Clear viewport
        gotoxy(xpos, ypos);
        cclear(width);

        // Print viweable part of string
        gotoxy(xpos, ypos);
        cprintf("%.*s", width - 1, str + offs);

        // Print cursor
        gotoxy(xpos + idx - offs, ypos);
        cputc((str[idx] ? 128 + str[idx] : CH_INVSPACE));

        // Debug
        //gotoxy(2, 2);
        //cprintf("I:%3u W:%3u L:%3u O:%3u S:%3u", idx, width, len, offs, size);

        // Get key value
        c = getkey(ijk_present);

        // Do action based on key pressed
        switch (c)
        {
        case CH_ESC:
            // ESC: Cancel input and return -1
            return -1;

        case CH_ENTER:
            // ENTER: Finish input and returns length plus changed string
            gotoxy(xpos, ypos);
            cclear(width);
            gotoxy(xpos, ypos);
            cprintf("%.*s", width - 1, str);
            return len;

        case CH_DEL:
            // DEL: Delete prev char
            if (idx)
            {
                --idx;
                for (c = idx; 1; ++c)
                {
                    unsigned char b = str[c + 1];
                    str[c] = b;
                    if (b == 0)
                    {
                        break;
                    }
                }
            }
            break;

        case CH_CURS_LEFT:
            if (idx)
            {
                --idx;
            }
            break;

        case CH_CURS_RIGHT:
            if (idx < strlen(str) && idx < size)
            {
                ++idx;
            }
            break;

        default:
            valid = 0;
            if (!validation)
            {
                valid = 1;
            }
            if ((validation & 1) && c > 47 && c < 58)
            {
                valid = 1;
            }
            if ((validation & 2) && c > 64 && c < 91)
            {
                valid = 1;
            }
            if ((validation & 2) && c > 96 && c < 123)
            {
                valid = 1;
            }
            if ((validation & 4) && (c == 42 || c == 63))
            {
                valid = 1;
            }
            if (isprint(c) && idx < size && valid)
            {
                unsigned char flag = (str[idx] == 0);
                str[idx] = c;
                if (flag)
                {
                    str[idx + 1] = 0;
                }
                idx++;
                break;
            }
            break;
        }
    }
    return 0;
}