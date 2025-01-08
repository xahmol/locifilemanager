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
    cputsxy(2, 18, "File:");
    gotoxy(2, 19);
    cprintf("%.30s", file);
    choice = menu_pulldown(10, 20, MENU_YESNO, 0);
    windowrestore();
    return (choice == 1) ? 1 : 0;
}

void file_copy_selected()
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

    if (presentdir[activepane].firstelement)
    {
        windownew(0, 8, 15);

        cputsxy(2, 9, "Copy files to:");
        gotoxy(2, 10);
        cprintf("%.35s", presentdir[target].path);

        element = presentdir[activepane].firstelement;

        cputsxy(2, 12, "Copying file:");
        cputsxy(2, 15, "Press ESC to cancel after next file.");

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
                    if (_sysremove(pathbuffer) != 0)
                    {
                        cputsxy(2, 12, "Error deleting file.");
                        break;
                    }
                }

                // Derive full source path
                strncpy(pathbuffer2, presentdir[activepane].path, sizeof(pathbuffer2));
                strncat(pathbuffer2, presentdirelement.name, sizeof(pathbuffer2) - strlen(pathbuffer2));

                // Copy file
                if (file_copy(pathbuffer, pathbuffer2, 1, 2, 14, 32) != 0)
                {
                    cputsxy(2, 14, "Error copying file.");
                    break;
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
            cputsxy(2, 13, "No files selected.");
        }

        gotoxy(2, 15);
        cclear(36);
        cputsxy(2, 15, "Press key.");
        getkey(ijk_present);

        windowrestore();

        // Draw new dirs
        dir_draw(target, 1);
        if (count)
        {
            dir_select_all(0);
        }
    }
}

void file_delete()
// Function to delete files from active pane
{
    unsigned char confirmed = 0;
    unsigned char count = 0;
    unsigned element;

    if (presentdir[activepane].firstelement)
    {
        windownew(0, 8, 15);

        cputsxy(2, 9, "Delete files:");

        element = presentdir[activepane].firstelement;

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
                if (_sysremove(pathbuffer) != 0)
                {
                    cputsxy(2, 12, "Error deleting file.");
                    break;
                }
            }
            element = presentdirelement.meta.next;
        } while (element);

        // Show message if no files selected
        if (!count)
        {
            cputsxy(2, 11, "No files selected.");
        }

        cputsxy(2, 15, "Press key.");
        getkey(ijk_present);

        windowrestore();

        // Draw new dirs
        dir_draw(activepane, 1);
    }
}

void file_rename()
// Function to rename selected file or dir
{
    char input[64];

    if (presentdir[activepane].firstelement)
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
                if (_sysrename(pathbuffer, pathbuffer2))
                {
                    // Print error message
                    cputsxy(2, 14, "Rename failed. Prsss key.");
                    getkey(ijk_present);
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
