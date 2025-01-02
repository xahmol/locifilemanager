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
    unsigned char exit = 0;
    unsigned char key;
    unsigned char len;
    activepane = 0;
    filter = 0;
    enterchoice = 0;
    confirm = 0;

    // Init
    setflags(SCREEN + NOKEYCLICK);
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);
    clrscr();
    get_locicfg();
    ijk_detect();

    // Place header and menubar
    menu_placetop("LOCI File Manager");
    sprintf(pathbuffer, "%s FW %s", locicfg.uname.sysname, locicfg.uname.release);
    len = strlen(pathbuffer);
    if (len > 20)
    {
        pathbuffer[20] = 0;
        len = 20;
    }
    gotoxy(39 - len, 0);
    cputs(pathbuffer);

    // Set start dirs and print
    strcpy(presentdir[0].path, "0:/");
    presentdir[0].drive = 0;
    dir_draw(0, 1);
    presentdir[1].drive = 0;
    dir_get_next_drive(1);

    // Main loop
    do
    {
        present = presentdir[activepane].present;
        if (present)
        {
            dir_get_element(present);
        }

        key = getkey(ijk_present);

        switch (key)
        {
        case CH_ENTER:
            // Enter: Enter directory or perfom selected action on file
            if (presentdirelement.meta.type == 1)
            {
                strcpy(pathbuffer, presentdir[activepane].path);
                strcat(pathbuffer, presentdirelement.name); 
                strcpy(presentdir[activepane].path, pathbuffer);
                dir_draw(activepane, 1);
            }
            else
            {
                // Perform action on file
                if(enterchoice==0)
                {
                    dir_select_toggle();
                }
            }
            break;

        case CH_ESC:
            // ESC: Application exit
            if (menu_areyousure("Exit application.") == 1)
            {
                exit = 1;
            }
            break;

        case '.':
            // .: Next drive for active pane
            dir_get_next_drive(activepane);
            break;

        case ',':
            // ,: Previous drive for active pane
            dir_get_prev_drive(activepane);
            break;

        case '/':
            // /: Previous drive for active pane
            dir_switch_pane();
            break;
        
        case '\\':
            // \: Go to root
            dir_gotoroot();
            break;

        case CH_CURS_LEFT:
            // Curs Left: Go to parent directory
            dir_parentdir();
            break;

        case CH_CURS_DOWN:
            // Curs Down: Scroll down
            dir_go_down();
            break;

        case CH_CURS_UP:
            // Curs Up: Scroll up
            dir_go_up();
            break;

        case 'd':
            // d: Page down
            dir_pagedown();
            break;

        case 'u':
            // u: Page up
            dir_pageup();
            break;
        
        case 't':
            // t: Go to top
            dir_top();
            break;

        case 'b':
            // b: Go to bottom
            dir_bottom();
            break;

        case 's':
            // s: Toggle select
            dir_select_toggle();
            break;

        case 'a':
            // a: Select all
            dir_select_all(1);
            break;

        case 'n':
            // n: Select none
            dir_select_all(0);
            break;

        case 'i':
            // i: Inverse selection
            dir_select_inverse();
            break;

        default:
            break;
        }
    } while (!exit);

    // Clear screen on exit
    setflags(SCREEN);
    clrscr();
}