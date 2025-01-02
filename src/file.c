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
    cputsxy(2, 19, truncate(file, 30));
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

    if (presentdir[activepane].firstelement)
    {
        windownew(0, 8, 15);

        cputsxy(2, 9, "Copy files to:");
        cputsxy(2, 10, truncate(presentdir[target].path, 35));

        element = presentdir[activepane].firstelement;

        cputsxy(2, 12, "Copying file:");

        do
        {
            dir_get_element(element);

            // Check if file is selected
            if (presentdirelement.meta.select)
            {
                count++;
                gotoxy(2, 13);
                cclear(34);
                cputsxy(2, 13, truncate(presentdirelement.name, 34));

                // Check if path gets too long
                if (roomleft < strlen(presentdirelement.name))
                {
                    cputsxy(2, 14, "Path gets too long.");
                    break;
                }

                // Derive full target path
                strcpy(pathbuffer, presentdir[target].path);
                strcat(pathbuffer, presentdirelement.name);

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
                strcpy(pathbuffer2, presentdir[activepane].path);
                strcat(pathbuffer2, presentdirelement.name);

                // Copy file
                if (file_copy(pathbuffer, pathbuffer2, 1, 13) != 0)
                {
                    cputsxy(2, 14, "Error copying file.");
                    break;
                }
            }
            element = presentdirelement.meta.next;
        } while (element);

        // Show message if no files selected
        if (!count)
        {
            cputsxy(2, 13, "No files selected.");
        }

        cputsxy(2, 15, "Press key.");
        getkey(1);

        windowrestore();

        // Draw new dirs
        dir_draw(target, 1);
        if(count)
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
                cputsxy(2, 11, truncate(presentdirelement.name, 34));

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
                strcpy(pathbuffer, presentdir[activepane].path);
                strcat(pathbuffer, presentdirelement.name);

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
        getkey(1);

        windowrestore();

        // Draw new dirs
        dir_draw(activepane, 1);
    }
}