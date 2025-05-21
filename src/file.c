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
#include "file.h"
#include "drive.h"

// Functions
unsigned char file_confirm_message(const char *message, const char *file)
// Function to show message if file exists
// Input: message = message to display
//        file = filename
// Output: 1 if user confirms, 0 if not
{
    unsigned char choice;

    windownew(0, 14, 9);
    cputsxy(2, 16, message);
    cputsxy(2, 18, "Name:");
    gotoxy(2, 19);
    cprintf("%.30s", file);
    choice = menu_pulldown(10, 20, MENU_YESNO, 0);
    windowrestore();
    return (choice == 1) ? 1 : 0;
}

void file_copy_move_selected(char move)
//  Function to copy files from active pane to non-active pane
{
    unsigned char confirmed = 0;
    unsigned char target = !activepane;
    unsigned char count = 0;
    unsigned element;
    unsigned char roomleft = 255 - strlen(presentdir[target].path);

    if (!strcmp(presentdir[target].path, presentdir[activepane].path))
    {
        menu_messagepopup("Target dir same as active dir.");
        return;
    }

    if (presentdir[activepane].firstelement && !insidetape[activepane])
    {
        windownew(0, 8, 15);

        // If nothing is selected and cursor pos is not a dir: select single file on cursor pos
        if (!selection[activepane] && presentdirelement.meta.type > 1)
        {
            presentdirelement.meta.select = 1;
            dir_save_element(present);
            selection[activepane]++;
        }

        // Print user info
        gotoxy(2, 9);
        cprintf("%s %u files to:", (move)?"Move":"Copy", selection[activepane]);
        gotoxy(2, 10);
        cprintf("%.35s", presentdir[target].path);

        element = presentdir[activepane].firstelement;

        cputsxy(2, 12, "Processing file:");
        cputsxy(2, 15, "Press ESC to cancel after next file.");

        // Loop through all elements to copy all selected files
        do
        {
            dir_get_element(element);

            // Check if file is selected
            if (presentdirelement.meta.select)
            {
                count++;
                gotoxy(2, 13);
                cclear(34);
                gotoxy(2, 13);
                cprintf("%.35s", presentdirelement.name);

                // Check if path gets too long
                if (roomleft < strlen(presentdirelement.name))
                {
                    cputsxy(2, 14, "Path gets too long.");
                    break;
                }

                // Derive full target path
                strncpy(pathbuffer, presentdir[target].path, sizeof(pathbuffer));
                strncat(pathbuffer, presentdirelement.name, sizeof(pathbuffer) - strlen(pathbuffer));

                // Check if file exists
                if (file_exists(pathbuffer))
                {
                    if (confirm || !confirmed)
                    {
                        if (!file_confirm_message("File exists. Overwrite?", presentdirelement.name))
                        {
                            break;
                        }
                        else
                        {
                            confirmed = 1;
                        }
                    }
                    // Delete file if user selects overwrite
                    if (remove(pathbuffer) != 0)
                    {
                        menu_fileerrormessage();
                        break;
                    }
                }

                // Derive full source path
                strncpy(pathbuffer2, presentdir[activepane].path, sizeof(pathbuffer2));
                strncat(pathbuffer2, presentdirelement.name, sizeof(pathbuffer2) - strlen(pathbuffer2));

                // Copy file
                if (file_copy(pathbuffer, pathbuffer2, 1, 2, 14, 32) != 0)
                {
                    menu_fileerrormessage();
                    break;
                }

                // Delete file if move is selected
                if (move)
                {
                    // Delete file
                    if (remove(pathbuffer2) != 0)
                    {
                        menu_fileerrormessage();
                        break;
                    }
                }

                // Check for pressing ESC to cancel
                if (kbhit())
                {
                    if (cgetc() == CH_ESC)
                    {
                        break;
                    }
                }
            }
            element = presentdirelement.meta.next;
        } while (element);

        // Show message if no files selected
        if (!count)
        {
            cputsxy(2, 13, "Nothing to copy.");
        }

        gotoxy(2, 15);
        cclear(36);
        cputsxy(2, 15, "Press key.");
        getkey(ijk_present);

        windowrestore();

        // Draw new dirs
        dir_draw(target, 1);
        if(move)
        {
            dir_draw(activepane, 1);
        }

        // Reset selection to 0
        if (count || selection[activepane])
        {
            dir_select_all(0);
            selection[activepane] = 0;
        }
    }
}

void file_delete()
// Function to delete files from active pane
{
    unsigned char confirmed = 0;
    unsigned char count = 0;
    unsigned element;

    if (presentdir[activepane].firstelement && !insidetape[activepane])
    {
        windownew(0, 8, 15);

        // If nothing is selected and cursor pos is not a dir: select single file on cursor pos
        if (!selection[activepane] && presentdirelement.meta.type > 1)
        {
            presentdirelement.meta.select = 1;
            dir_save_element(present);
            selection[activepane]++;
        }

        gotoxy(2, 9);
        cprintf("Delete %u files:", selection[activepane]);

        element = presentdir[activepane].firstelement;

        // Loop through all elements to delete all selected files
        do
        {
            dir_get_element(element);

            // Check if file is selected
            if (presentdirelement.meta.select)
            {
                count++;
                gotoxy(2, 11);
                cclear(34);
                gotoxy(2, 11);
                cprintf("%.35s", presentdirelement.name);

                // Confirm delete
                if (confirm || !confirmed)
                {
                    if (!file_confirm_message("Delete file?", presentdirelement.name))
                    {
                        break;
                    }
                    else
                    {
                        confirmed = 1;
                    }
                }

                // Derive full target path
                strncpy(pathbuffer, presentdir[activepane].path, sizeof(pathbuffer));
                strncat(pathbuffer, presentdirelement.name, sizeof(pathbuffer) - strlen(pathbuffer));

                // Delete file
                if (remove(pathbuffer) != 0)
                {
                    menu_fileerrormessage();
                    break;
                }
            }
            element = presentdirelement.meta.next;
        } while (element);

        // Show message if no files selected
        if (!count)
        {
            cputsxy(2, 11, "No files to delete.");
        }

        cputsxy(2, 15, "Press key.");
        getkey(ijk_present);

        windowrestore();

        // Draw new dirs
        dir_draw(activepane, 1);

        // Reset selection to 0
        if (count || selection[activepane])
        {
            dir_select_all(0);
            selection[activepane] = 0;
        }
    }
}

void file_rename()
// Function to rename selected file or dir
{
    char input[64];

    if (presentdir[activepane].firstelement && !insidetape[activepane])
    {
        windownew(0, 8, 15);

        cputsxy(2, 9, "Rename file.");

        strncpy(input, presentdirelement.name, 65);

        // Is it a directory? Then remove trailing /
        if (presentdirelement.meta.type == 1)
        {
            input[strlen(input) - 1] = 0;
        }

        // Set old name full path
        strncpy(pathbuffer, presentdir[activepane].path, sizeof(pathbuffer));
        strncat(pathbuffer, input, sizeof(pathbuffer) - strlen(pathbuffer));

        cputsxy(2, 11, "Enter new name:");

        // Input new name and check if not cancelled or empty string
        if (textInput(2, 12, 35, input, 64, 0) > 0)
        {
            // Check if name is actually altered
            if (strcmp(input, presentdirelement.name))
            {
                // Set new name full path
                strncpy(pathbuffer2, presentdir[activepane].path, sizeof(pathbuffer2));
                strncat(pathbuffer2, input, sizeof(pathbuffer2) - strlen(pathbuffer2));

                // Rename
                if (rename(pathbuffer, pathbuffer2))
                {
                    // Print error message
                    menu_fileerrormessage();
                    windowrestore();
                    return;
                }
                else
                {
                    // Redraw dir.
                    // Neceasary as name length can be changed, so all address need to be changed.
                    windowrestore();
                    dir_draw(activepane, 1);
                    return;
                }
            }
        }
        windowrestore();
    }
}

void file_browse_tape()
// Function to browse a .TAP tape archive
{
    if (presentdir[activepane].firstelement && !insidetape[activepane] && presentdirelement.meta.type == 3)
    {
        drive_mount();
        insidetape[activepane] = 1;
        dir_draw(activepane, 1);
    }
}
