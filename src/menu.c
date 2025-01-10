// Includes
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <peekpoke.h>
#include <ctype.h>
#include <atmos.h>
#include <errno.h>
#include "osdklib.h"
#include "loci.h"
#include "defines.h"
#include "generic.h"
#include "menu.h"
#include "dir.h"

// Variables
// Windows
struct WindowStruct Window[9];
char *windowaddress = (char *)WINDOWBASEADDRESS;
unsigned char windownumber = 0;

// Menus
struct MenuBar menubar = {
    {"App", "File", "Dir", "Mounts", "Info"},
    {0, 0, 0, 0, 0},
    0};

char pulldown_options[PULLDOWN_NUMBER] = {5, 8, 7, 7, 2, 3, 5, 4, 7, 2};
char pulldown_titles[PULLDOWN_NUMBER][PULLDOWN_MAXOPTIONS][PULLDOWN_MAXLENGTH] = {
    {"Confirm:  Once  ",
     "Enter:    Select",
     "[F]ilter: None  ",
     "S[O]rt:   Off   ",
     "[ESC] Exit      "},
    {"[S]elect toggle ",
     "Select [A]ll    ",
     "Select [N]one   ",
     "[I]nverse select",
     "[DEL]ete        ",
     "[R]ename        ",
     "[C]opy          ",
     "Bro[W]se tape   "},
    {"[/] Go to root",
     "[C LEFT] Back ",
     "Page [D]own   ",
     "Page U[P]     ",
     "[T]op         ",
     "[B]ottom      ",
     "N[e]w dir     "},
    {"[/] Switch pane",
     "[.] Next drive ",
     "[,] Prev drive ",
     "[M]ount        ",
     "[U]nmount      ",
     "Tar[G]et: A    ",
     "Show mounts    "},
    {"Version/credits",
     "Help           "},
    {"Select",
     "Mount ",
     "Launch"},
    {"None  ",
     ".DSK  ",
     ".TAP  ",
     ".ROM  ",
     ".LCE  "},
    {"A    ",
     "B    ",
     "C    ",
     "D    "},
    {"A   ",
     "B   ",
     "C   ",
     "D   ",
     "Tape",
     "ROM ",
     "None"},
    {"Yes",
     "No "}};

// Functions
// Windows
void windowsave(unsigned char ypos, unsigned char height)
// Function to save a window
// Input:
// - ypos: startline of window
// - height: height of window
{
    char *baseaddress = (char *)TEXTVRAM + (ypos * 40);
    int length = height * 40;

    Window[windownumber].address = windowaddress;
    Window[windownumber].ypos = ypos;
    Window[windownumber].height = height;

    // Copy screen
    enable_overlay_ram();
    memcpy(windowaddress, baseaddress, length);
    disable_overlay_ram();
    windowaddress += length;

    windownumber++;
}

void windowrestore()
// Function to restore a window
{
    char *baseaddress = (char *)TEXTVRAM + (Window[--windownumber].ypos * 40);
    int length = Window[windownumber].height * 40;

    windowaddress = Window[windownumber].address;

    // Restore screem
    enable_overlay_ram();
    memcpy(baseaddress, windowaddress, length);
    disable_overlay_ram();
}

void windownew(unsigned char xpos, unsigned char ypos, unsigned char height)
// Function to make menu border
// Input:
// - xpos: x-coordinate of left upper corner
// - ypos: y-coordinate of right upper corner
// - height: number of rows in window
// - width not required, window will always be to full right
{
    unsigned char y;

    windowsave(ypos, height);
    for (y = 0; y < height; y++)
    {
        gotoxy(xpos, ypos + y);
        cputc(A_BGWHITE);
        cputc(A_FWBLACK);
        cclear(38 - xpos);
    }
}

// Menus
void menu_placeheader(const char *header)
// Print header title
// Input: header = title to print
{
    gotoxy(0, 0);
    cputc(A_FWBLACK);
    cputc(A_BGGREEN);
    cclear(38);
    gotoxy(2, 0);
    cputs(header);
}

void menu_placebar(unsigned char y)
// Place main meny bar at given line ycoord
// Input: y = y-coordinate of menu bar
{
    unsigned char xcoord = 1;
    unsigned char len, i;

    menubar.ypos = y;

    gotoxy(0, y);
    cputc(A_FWBLACK);
    cputc(A_BGGREEN);
    cclear(38);

    for (i = 0; i < MENUBAR_MAXOPTIONS; i++)
    {
        len = strlen(menubar.titles[i]);
        if (xcoord + len > 39)
        {
            xcoord = 39 - len;
        }
        cputsxy(xcoord + 1, y, menubar.titles[i]);
        menubar.xstart[i] = xcoord;
        xcoord += len + 2;
    }
}

void menu_placetop(const char *header)
// Place both header and menubar at lines 0 and 1
// Input: header = title to print
{
    clrscr();
    menu_placeheader(header);
    menu_placebar(1);
}

unsigned char menu_pulldown(unsigned char xpos, unsigned char ypos, unsigned char menunumber, unsigned char escapable)
// Function for pull down menu
// Input:
// - xpos = x-coordinate of upper left corner
// - ypos = y-coordinate of upper left corner
// - menunumber =
//   number of the menu as defined in pulldownmenuoptions array
// - espacable: ability to escape with escape key enabled (1) or not (0)
// Output: menu choice number
{
    unsigned char y;
    unsigned char key;
    unsigned char exit = 0;
    unsigned char menuchoice = 1;
    unsigned char height = pulldown_options[menunumber];
    unsigned char topmenu = (menunumber > MENUBAR_MAXOPTIONS - 1) ? 0 : 1;
    unsigned char endcolor = (topmenu) ? A_BGBLACK : A_BGWHITE;

    windowsave(ypos, pulldown_options[menunumber]);
    for (y = 0; y < pulldown_options[menunumber]; y++)
    {
        gotoxy(xpos - 1, ypos + y);
        cputc(A_FWBLACK);
        cputc(A_BGCYAN);
        cprintf(" %s ", pulldown_titles[menunumber][y]);
        cputc(endcolor);
    }

    do
    {
        gotoxy(xpos - 1, ypos + menuchoice - 1);
        cputc(A_FWBLACK);
        cputc(A_BGYELLOW);
        cprintf("-%s ", pulldown_titles[menunumber][menuchoice - 1]);
        cputc(endcolor);

        do
        {
            key = getkey(ijk_present);
        } while (key != CH_ENTER && key != CH_CURS_LEFT && key != CH_CURS_RIGHT && key != CH_CURS_UP && key != CH_CURS_DOWN && key != CH_ESC && key != CH_STOP);

        switch (key)
        {
        case CH_ESC:
            if (escapable == 1)
            {
                exit = 1;
                menuchoice = 0;
            }
            break;

        case CH_ENTER:
            exit = 1;
            break;

        case CH_CURS_LEFT:
            if (topmenu)
            {
                exit = 1;
                menuchoice = 18;
            }
            break;

        case CH_CURS_RIGHT:
            if (topmenu)
            {
                exit = 1;
                menuchoice = 19;
            }
            break;

        case CH_CURS_DOWN:
        case CH_CURS_UP:
            gotoxy(xpos - 1, ypos + menuchoice - 1);
            cputc(A_FWBLACK);
            cputc(A_BGCYAN);
            cprintf(" %s ", pulldown_titles[menunumber][menuchoice - 1]);
            cputc(endcolor);
            if (key == CH_CURS_UP)
            {
                menuchoice--;
                if (menuchoice < 1)
                {
                    menuchoice = height;
                }
            }
            else
            {
                menuchoice++;
                if (menuchoice > height)
                {
                    menuchoice = 1;
                }
            }
            break;

        default:
            break;
        }
    } while (exit == 0);

    windowrestore();

    return menuchoice;
}

unsigned char menu_main()
// Function for main menu selection
// Output: menu choice number
{
    unsigned char menubarchoice = 1;
    unsigned char menuoptionchoice = 0;
    unsigned char key;
    unsigned char xpos;

    menu_placebar(menubar.ypos);

    do
    {
        do
        {
            gotoxy(menubar.xstart[menubarchoice - 1], menubar.ypos);
            cputc(A_BGWHITE);
            cprintf("%s ", menubar.titles[menubarchoice - 1]);
            cputc(A_BGGREEN);

            do
            {
                key = getkey(ijk_present);
            } while (key != CH_ENTER && key != CH_CURS_LEFT && key != CH_CURS_RIGHT && key != CH_ESC && key != CH_STOP);

            gotoxy(menubar.xstart[menubarchoice - 1], menubar.ypos);
            cputc(A_BGGREEN);
            cprintf("%s ", menubar.titles[menubarchoice - 1]);

            if (key == CH_CURS_LEFT)
            {
                menubarchoice--;
                if (menubarchoice < 1)
                {
                    menubarchoice = MENUBAR_MAXOPTIONS;
                }
            }
            else if (key == CH_CURS_RIGHT)
            {
                menubarchoice++;
                if (menubarchoice > MENUBAR_MAXOPTIONS)
                {
                    menubarchoice = 1;
                }
            }
        } while (key != CH_ENTER && key != CH_ESC && key != CH_STOP);
        if (key != CH_ESC && key != CH_STOP)
        {
            xpos = menubar.xstart[menubarchoice - 1];
            if (xpos + strlen(pulldown_titles[menubarchoice - 1][0]) > 36)
            {
                xpos = 36 - strlen(pulldown_titles[menubarchoice - 1][0]);
            }
            menuoptionchoice = menu_pulldown(xpos, menubar.ypos + 1, menubarchoice - 1, 1);
            if (menuoptionchoice == 18)
            {
                menuoptionchoice = 0;
                menubarchoice--;
                if (menubarchoice < 1)
                {
                    menubarchoice = MENUBAR_MAXOPTIONS;
                }
            }
            if (menuoptionchoice == 19)
            {
                menuoptionchoice = 0;
                menubarchoice++;
                if (menubarchoice > MENUBAR_MAXOPTIONS)
                {
                    menubarchoice = 1;
                }
            }
        }
        else
        {
            menuoptionchoice = 99;
        }
    } while (menuoptionchoice == 0);

    return menubarchoice * 10 + menuoptionchoice;
}

unsigned char menu_areyousure(const char *message)
// Pull down menu to verify if user is sure
// Input: message = message to display
// Output: 1 if yes, 2 if no
{
    unsigned char choice;

    windownew(5, 8, 6);
    cputsxy(7, 9, message);
    cputsxy(7, 10, "Are you sure?");
    choice = menu_pulldown(20, 11, MENU_YESNO, 0);
    windowrestore();
    return choice;
}

void menu_fileerrormessage()
// Show message for file error encountered
// Input: error = error number
{
    windownew(5, 8, 8);

    cputsxy(7, 9, "File error!");

    gotoxy(7, 11);
    cprintf("Error# : %d", errno);
    gotoxy(7,12);
    cprintf("Message: %.30s", strerror(errno));

    cputsxy(7, 13, "Press key.");
    getkey(ijk_present);

    windowrestore();
}

void menu_messagepopup(const char *message)
// Show popup with a message
// Input: message = message to display
{
    windownew(5, 8, 6);

    cputsxy(7, 9, message);

    cputsxy(7, 11, "Press key.");
    getkey(ijk_present);

    windowrestore();
}

unsigned char menu_option_select(const char *message, unsigned char menu)
// Select option from pulldown
{
    unsigned char option;
    windownew(5, 8, 12);
    cputsxy(7, 9, message);
    cputsxy(7, 11, "Select option:");
    option = menu_pulldown(10, 12, menu, 1);
    windowrestore();
    return option;
}
