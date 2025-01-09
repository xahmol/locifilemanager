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
#include "file.h"
#include "drive.h"

// Global variables
char buffer[41];
char version[22];
unsigned char done;

// Main menu operation
void confirm_toggle()
// Toggle confirm once or all
{
    confirm = !confirm;
    if (confirm)
    {
        strcpy(buffer, "All   ");
    }
    else
    {
        strcpy(buffer, "Once  ");
    }
    strncpy(pulldown_titles[0][0] + 10, buffer, PULLDOWN_MAXLENGTH - 10);
}

void select_enter_choice()
// Select what to do on enter
{
    unsigned char select;

    select = menu_option_select("Action for enter",5);

    if (select)
    {
        enterchoice = select - 1;
        strncpy(pulldown_titles[0][1] + 10, pulldown_titles[5][enterchoice], PULLDOWN_MAXLENGTH - 10);
    }
}

void select_filter()
// Select which filter to apply
{
    unsigned char select;

    select = menu_option_select("Apply filter",6);

    if (select)
    {
        filter = select - 1;
        strncpy(pulldown_titles[0][2] + 10, pulldown_titles[6][filter], PULLDOWN_MAXLENGTH - 10);
        dir_draw(0, 1);
        dir_draw(1, 1);
    }
}

void versioninfo()
{
    windownew(2, 5, 15);
    cputsxy(4, 6, "Version information and credits");
    cputsxy(4, 8, "LOCI FIle Manager");
    cputsxy(4, 9, "Written in 2024 by Xander Mol");
    sprintf(buffer, "version: %s", version);
    cputsxy(4, 11, buffer);
    cputsxy(4, 13, "Source, docs and credits at:");
    cputsxy(4, 14, "github.com/xahmol/locifilemanager");
    cputsxy(4, 16, "(C) 2024, IDreamtIn8bits.com");
    cputsxy(4, 18, "Press a key to continue.");
    getkey(ijk_present);
    windowrestore();
}

void mainmenuloop()
// Go to main menu and act on selection made
{
    unsigned char choice;

    do
    {
        present = presentdir[activepane].present;
        if (present)
        {
            dir_get_element(present);
        }

        choice = menu_main();

        switch (choice)
        {
        case 11:
            confirm_toggle();
            break;

        case 12:
            select_enter_choice();
            break;

        case 13:
            select_filter();
            break;

        case 14:
            dir_togglesort();
            break;

        case 15:
            if (menu_areyousure("Exit application.") == 1)
            {
                done = 1;
                choice = 99;
            }
            break;

        case 21:
            dir_select_toggle();
            break;

        case 22:
            dir_select_all(1);
            break;

        case 23:
            dir_select_all(0);
            break;

        case 24:
            dir_select_inverse();
            break;

        case 25:
            file_delete();
            break;

        case 26:
            file_rename();
            break;

        case 27:
            file_copy_selected();
            break;

        case 31:
            dir_gotoroot();
            break;

        case 32:
            dir_parentdir();
            break;

        case 33:
            dir_pagedown();
            break;

        case 34:
            dir_pageup();
            break;

        case 35:
            dir_top();
            break;

        case 36:
            dir_bottom();
            break;

        case 37:
            break;

        case 41:
            dir_switch_pane();
            break;

        case 42:
            dir_get_next_drive(activepane);
            break;

        case 43:
            dir_get_prev_drive(activepane);
            break;

        case 44:
            drive_mount();
            break;

        case 45:
            drive_unmount();
            break;

        case 46:
            drive_targetdrive();
            break;

        case 47:
            drive_showmounts();
            break;

        case 51:
            versioninfo();
            break;

        case 52:
            break;

        default:
            break;
        }
    } while (choice < 99);
}

// Main loop
void main()
{
    unsigned char key;
    unsigned char len;
    done = 0;
    activepane = 0;
    filter = 0;
    enterchoice = 0;
    confirm = 0;
    sort = 0;
    targetdrive = 0;

    // Set version number in string variable
    sprintf(version,
            "v%2i.%2i - %c%c%c%c%c%c%c%c-%c%c%c%c",
            VERSION_MAJOR, VERSION_MINOR,
            BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3, BUILD_MONTH_CH0, BUILD_MONTH_CH1, BUILD_DAY_CH0, BUILD_DAY_CH1, BUILD_HOUR_CH0, BUILD_HOUR_CH1, BUILD_MIN_CH0, BUILD_MIN_CH1);

    // Init
    setflags(SCREEN + NOKEYCLICK);
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);
    clrscr();
    get_locicfg();
    ijk_detect();

    // Unmount all drives, tape and ROM to ensure a known state
    drive_unmount_all();

    // Testing input
    // cputsxy(2,4,"012345678901234567890123456789012345");
    // strcpy(pathbuffer,"0123456789");
    // textInput(2,5,35,pathbuffer,64,0);
    // strcpy(pathbuffer,"012345678901234567890123456789012345678901234567890123456789");
    // textInput(2,6,35,pathbuffer,64,0);
    // gotoxy(2,7);
    // cputs(pathbuffer);
    // cgetc();
    // clrscr();

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
        case CH_CURS_RIGHT:
            // Go to menu
            mainmenuloop();
            break;

        case CH_ENTER:
            // Enter: Enter directory or perfom selected action on file
            if (presentdir[activepane].firstelement && presentdirelement.meta.type == 1)
            {
                strncpy(pathbuffer, presentdir[activepane].path, sizeof(pathbuffer));
                strncat(pathbuffer, presentdirelement.name, sizeof(pathbuffer) - strlen(pathbuffer));
                strncpy(presentdir[activepane].path, pathbuffer, sizeof(presentdir[activepane].path));
                dir_draw(activepane, 1);
            }
            else
            {
                // Perform action on file
                switch (enterchoice)
                {
                case 0:
                    dir_select_toggle();
                    break;
                
                case 1:
                    drive_mount();
                    break;

                case 2:
                    break;
                
                default:
                    break;
                }
            }
            break;

        case CH_ESC:
            // ESC: Application exit
            if (menu_areyousure("Exit application.") == 1)
            {
                done = 1;
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
            // /: Switch pane
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

        case 'p':
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

        case 'o':
            // o: Toggle sort
            dir_togglesort();
            break;

        case 'c':
            // c: Copy files
            file_copy_selected();
            break;

        case 'f':
            // f: Select filter
            select_filter();
            break;

        case 'g':
            // g: select target drive
            drive_targetdrive();
            break;

        case 'r':
            // r: Rename
            file_rename();
            break;

        case 'm':
            // m: mount
            drive_mount();
            break;

        case 'u':
            // u: unmount
            drive_unmount();
            break;

        case CH_DEL:
            // Del: Delete files
            file_delete();
            break;

        default:
            break;
        }
    } while (!done);

    // Clear screen on exit
    setflags(SCREEN);
    clrscr();
}