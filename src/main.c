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
#include "menu.h"

// Global variables
char buffer[81];
char version[22];

// Generic routines
void wait(unsigned int wait_cycles)
// Function to wait for the specified number of cycles
// Input: wait_cycles = numnber of cycles to wait
{

    unsigned int starttime = clock();
    while (clock() - starttime < wait_cycles)
        ;
}

// Generic input routines

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

// Main loop
void main()
{
    unsigned char key = 0;

    // Init
    ijk_detect();
    setflags(SCREEN + NOKEYCLICK);
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);
    clrscr();

    do
    {
        sprintf(buffer,"Key pressed: %02X",key);
        cputsxy(2,0,buffer);
        key = getkey(1);
    } while (key != CH_ESC);

    // Clear screen on exit
    setflags(SCREEN);
    clrscr();
}