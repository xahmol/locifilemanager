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
#include "dir.h"

// Global variables
char buffer[41];
char pathbuffer[256];
char version[22];

// Main loop
void main()
{
    unsigned char choice = 0;
    activepane = 0;
    filter = 0;

    // Init
    ijk_detect();
    get_locicfg();
    setflags(SCREEN + NOKEYCLICK);
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);
    clrscr();
    menu_placetop("LOCI File Manager");

    // Set start dirs and print
    strcpy(presentdir[0].path, "0:/");
    presentdir[0].drive = 0;
    dir_draw(0, 1);
    strcpy(presentdir[1].path, "1:/");
    presentdir[1].drive = 1;
    dir_draw(1, 1);
    cgetc();

    choice = menu_main();

    cputsxy(2, 4, "Choice made: ");
    cprintf("%2u", choice);

    cputsxy(2, 6, "Press a key to exit.");
    getkey(1);

    // Clear screen on exit
    setflags(SCREEN);
    clrscr();
}