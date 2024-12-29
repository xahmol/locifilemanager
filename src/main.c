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

// Main loop
void main()
{
    unsigned char choice = 0;

    // Init
    ijk_detect();
    setflags(SCREEN + NOKEYCLICK);
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);
    clrscr();

    menu_placetop("LOCI File Manager");
    choice = menu_main();

    cputsxy(2,4,"Choice made: ");
    cprintf("%2u",choice);

    cputsxy(2,6,"Press a key to exit.");
    getkey(1);

    // Clear screen on exit
    setflags(SCREEN);
    clrscr();
}