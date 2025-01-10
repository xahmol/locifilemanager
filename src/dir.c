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

// Variables
struct DirElement presentdirelement;
struct Directory presentdir[2];
DIR *dir;
struct dirent *file;
unsigned char activepane;
unsigned present;
unsigned previous;
unsigned next;
unsigned char filter;
unsigned char enterchoice;
unsigned char confirm;
unsigned char sort;
unsigned char targetdrive;
int selection;
char *diraddress[2] = {(char *)DIR1BASE, (char *)DIR2BASE};

char dir_entry_types[6][4] =
    {
        "DIR",
        "DSK",
        "TAP",
        "ROM",
        "LCE",
        "   "};

char pathbuffer[256];
char pathbuffer2[256];

// Functions
void dir_get_element(unsigned address)
// Get directory element from address
// Input: address = address of element
{
    unsigned workaddress = address;
    xram_memcpy_from(&presentdirelement.meta, workaddress, sizeof(presentdirelement.meta));
    workaddress += sizeof(presentdirelement.meta);
    xram_memcpy_from(presentdirelement.name, workaddress, presentdirelement.meta.length);
}

void dir_save_element(unsigned address)
// Save directory element to address
// Input: address = address of element
{
    unsigned workaddress = address;
    xram_memcpy_to(workaddress, &presentdirelement.meta, sizeof(presentdirelement.meta));
    workaddress += sizeof(presentdirelement.meta);
    xram_memcpy_to(workaddress, presentdirelement.name, presentdirelement.meta.length);
}

void dir_read(unsigned char dirnr, unsigned char filter)
// Read directory entries to memory from present path
// Input: dirnr = directory number
//        filter = filter for file type
{
    unsigned char presenttype;
    unsigned char datalength;
    unsigned char count = 0;
    unsigned char xpos = 3;
    unsigned char ypos = (dirnr) ? PANE2_YPOS : PANE1_YPOS;
    unsigned char bg_color;
    unsigned element;
    struct DirElement bufferdir;
    unsigned workaddress;
    unsigned char inserted;

    // Set colors based on whether pane is active or not
    bg_color = (activepane == dirnr) ? A_BGYELLOW : A_BGWHITE;
    ypos++;

    // Prepare progress bar
    gotoxy(0, ypos);
    cputc(A_FWBLACK);
    cputc(bg_color);
    cputc(A_ALT);
    cclear(37);

    previous = 0;

    // Debug
    // unsigned char i;

    // Reset dirdata
    presentdir[dirnr].address = (dirnr) ? DIR2BASE : DIR1BASE;
    present = presentdir[dirnr].address;

    // Clear directory values
    presentdir[dirnr].firstelement = 0;
    presentdir[dirnr].firstprint = 0;
    presentdir[dirnr].lastprint = 0;
    presentdir[dirnr].position = 0;
    presentdir[dirnr].present = 0;

    // Debug: Listing root dir drive 0
    // gotoxy(0,3);
    // dir = opendir(presentdir[dirnr].path);
    // if (!dir)
    //{
    //    return;
    //}
    //
    // while (file = readdir(dir), file->d_name[0] != 0)
    //{
    //    cprintf("FD:%2X AT:%2X %s (%lu) %s\n\r", file->d_fd, file->d_attrib, file->d_name, file->d_size,(file->d_attrib&DIR_ATTR_DIR)?"DIR":"");
    //}
    // closedir(dir);
    // cgetc();

    // Initialise reading dir
    dir = opendir(presentdir[dirnr].path);
    if (!dir)
    {
        return;
    }

    // Loop while dir data is available or memory is full
    while (file = readdir(dir), file->d_name[0] != 0)
    {
        // Debug
        // cleararea(16, 12);
        // gotoxy(0, 16);
        // cprintf("FD:%2X AT:%2X %s (%lu) %s\n\r", file->d_fd, file->d_attrib, file->d_name, file->d_size,(file->d_attrib&DIR_ATTR_DIR)?"DIR":"");

        // Reset entry type
        presenttype = 0;

        // Get next dir entry
        datalength = strlen(file->d_name);

        // print progress counter
        if ((count >> 2) > 36)
        {
            xpos = 3;
            count = 0;
            gotoxy(xpos, ypos);
            cclear(37);
        }
        else
        {
            gotoxy(xpos + (count >> 2), ypos);
            cputc(progressBar[count & 3]);
            ++count;
        }

        // Check if entry is a dir by checking if bit 4 of first byte is set
        if (file->d_attrib & DIR_ATTR_DIR)
        {
            presenttype = 1;

            // Add / to dir name if room is left
            if (datalength < 64)
            {
                datalength++;
                strcat(file->d_name, "/");
            }
        }

        // Debug
        // gotoxy(0, 18);
        // cprintf("Name: %s\n\r", file->d_name);
        // cprintf("Length: %2u ", datalength);

        // Check if file is a matching image type
        if (!presenttype && datalength > 4)
        {

            // Check for filename extension for file type
            // First check for a dot four chars from the end
            if (file->d_name[datalength - 4] == '.')
            {
                // Check for DSK
                if ((file->d_name[datalength - 3] == 'd' || file->d_name[datalength - 3] == 'D') &&
                    (file->d_name[datalength - 2] == 's' || file->d_name[datalength - 2] == 'S') &&
                    (file->d_name[datalength - 1] == 'k' || file->d_name[datalength - 1] == 'K'))
                {
                    presenttype = 2;
                }

                // Check for TAP
                if ((file->d_name[datalength - 3] == 't' || file->d_name[datalength - 3] == 'T') &&
                    (file->d_name[datalength - 2] == 'a' || file->d_name[datalength - 2] == 'A') &&
                    (file->d_name[datalength - 1] == 'p' || file->d_name[datalength - 1] == 'P'))
                {
                    presenttype = 3;
                }

                // Check for ROM
                if ((file->d_name[datalength - 3] == 'r' || file->d_name[datalength - 3] == 'R') &&
                    (file->d_name[datalength - 2] == 'o' || file->d_name[datalength - 2] == 'O') &&
                    (file->d_name[datalength - 1] == 'm' || file->d_name[datalength - 1] == 'M'))
                {
                    presenttype = 4;
                }

                // Check for LCE
                if ((file->d_name[datalength - 3] == 'l' || file->d_name[datalength - 3] == 'L') &&
                    (file->d_name[datalength - 2] == 'c' || file->d_name[datalength - 2] == 'C') &&
                    (file->d_name[datalength - 1] == 'e' || file->d_name[datalength - 1] == 'E'))
                {
                    presenttype = 5;
                }
            }
        }

        // Set file type to unknown if not set yet
        if (!presenttype)
        {
            presenttype = 6;
        }

        // Filter out hidden files
        if (file->d_name[0] == '.')
        {
            presenttype = 0;
        }

        // Filter out non-matching file types if filter is set
        if (filter && presenttype > 1)
        {
            if (presenttype != filter + 1)
            {
                presenttype = 0;
            }
        }

        // Debug
        // cprintf("Type: %u\n\r", presenttype);

        if (presenttype)
        {
            // Check if sufficent memory is left
            if (presentdir[dirnr].address + datalength + sizeof(presentdirelement.meta) > presentdir[dirnr].address + DIRSIZE - 1)
            {
                return;
            }

            // Set direntry pointers
            // Is this the first entry?
            if (!previous)
            {
                presentdir[dirnr].firstelement = present;
                presentdir[dirnr].firstprint = present;
                presentdirelement.meta.prev = 0;
                previous = present;
                presentdirelement.meta.next = 0;
            }
            // All next entries
            else
            {
                // Does the list need to be sorted?
                if (sort)
                {
                    inserted = 0;
                    // Find element to insert after
                    element = presentdir[dirnr].firstelement;
                    do
                    {
                        workaddress = element;
                        xram_memcpy_from(&bufferdir.meta, workaddress, sizeof(bufferdir.meta));
                        workaddress += sizeof(bufferdir.meta);
                        xram_memcpy_from(bufferdir.name, workaddress, bufferdir.meta.length);

                        // Debug
                        // cleararea((activepane)?5:17, 10);
                        // gotoxy(0,(activepane)?5:17);
                        // cprintf("Element: %4x Present: %4x\n\r", element, present);
                        // cprintf("To insert:\n\r%s\n\r", file->d_name);
                        // cprintf("Compare with:"
                        //       "\n\r%s\n\r",
                        //       bufferdir.name);

                        if (strncmp(bufferdir.name, file->d_name, 32) > 0)
                        {
                            // Insert before the first one?
                            if (!bufferdir.meta.prev)
                            {
                                presentdirelement.meta.prev = 0;
                                presentdirelement.meta.next = element;
                                bufferdir.meta.prev = present;
                                presentdir[dirnr].firstelement = present;
                                presentdir[dirnr].firstprint = present;
                                xram_memcpy_to(element, &bufferdir.meta, sizeof(bufferdir.meta));

                                // cprintf("Insert before first.\n\r");
                                // cprintf("Prev: %4x Next: %4x BuffPrev: %4x\n\r", presentdirelement.meta.prev, presentdirelement.meta.next, bufferdir.meta.prev);
                            }
                            else
                            // Insert in between
                            {
                                presentdirelement.meta.prev = bufferdir.meta.prev;
                                presentdirelement.meta.next = element;
                                bufferdir.meta.prev = present;
                                xram_memcpy_to(element, &bufferdir.meta, sizeof(bufferdir.meta));

                                // cprintf("Insert in betweent.\n\r");
                                // cprintf("Prev: %4x Next: %4x BuffNPrev: %4x ", presentdirelement.meta.prev, presentdirelement.meta.next, bufferdir.meta.prev);

                                xram_memcpy_from(&bufferdir.meta, presentdirelement.meta.prev, sizeof(bufferdir.meta));
                                bufferdir.meta.next = present;
                                // cprintf("BuffPNext: %4x\n\r", bufferdir.meta.next);
                                xram_memcpy_to(presentdirelement.meta.prev, &bufferdir.meta, sizeof(bufferdir.meta));
                            }
                            inserted = 1;
                            break;
                        }
                        element = bufferdir.meta.next;
                    } while (element);

                    // Insert at the ned
                    if (!inserted)
                    {
                        // cprintf("Insert at the end.\n\r");

                        presentdirelement.meta.prev = previous;              // Set prev in new entry
                        xram_memcpy_to(previous, &present, sizeof(present)); // Set next in previous entry
                        previous = present;
                        presentdirelement.meta.next = 0;
                    }
                    // cgetc();
                }
                else
                {
                    presentdirelement.meta.prev = previous;              // Set prev in new entry
                    xram_memcpy_to(previous, &present, sizeof(present)); // Set next in previous entry
                    previous = present;
                    presentdirelement.meta.next = 0;
                }

                // Debug
                // cprintf("Dump previous:\n\r");
                // for (i = 0; i < (present - previous); i++)
                //{
                //    cprintf("%2x", xram_peek(previous + i));
                //}
            }

            // Debug
            // cprintf("\n\rPrev: %4x Pres: %4x ", previous, present);

            presentdirelement.meta.select = 0;
            presentdirelement.meta.type = presenttype;
            datalength++;
            presentdirelement.meta.length = datalength;

            // Set meta data
            xram_memcpy_to(presentdir[dirnr].address, &presentdirelement.meta, sizeof(presentdirelement.meta));
            presentdir[dirnr].address += sizeof(presentdirelement.meta);

            // Set filename
            xram_memcpy_to(presentdir[dirnr].address, file->d_name, datalength);
            presentdir[dirnr].address += datalength;

            // Update present pointer
            present = presentdir[dirnr].address;

            // Debug
            // cprintf("Next: %4x\n\r", present);
            // cprintf("Dump present:\n\r");
            // for (i = 0; i < (present - previous); i++)
            //{
            //    cprintf("%2x", xram_peek(previous + i));
            //}
            // cgetc();
        }
    }
    closedir(dir);

    if (presentdir[dirnr].firstelement)
    {
        present = presentdir[dirnr].firstelement;
        dir_get_element(present);
    }
    else
    {
        present = 0;
    }

    dir_print_id_and_path(dirnr);
}

void dir_print_id_and_path(unsigned char dirnr)
// Draw drive ID and path
// Input: dirnr = directory number
{
    unsigned char ypos = (dirnr) ? PANE2_YPOS : PANE1_YPOS;
    unsigned char bg_color;

    // Set colors based on whether pane is active or not
    bg_color = (activepane == dirnr) ? A_BGYELLOW : A_BGWHITE;

    // Print device name
    gotoxy(0, ypos);
    cputc(A_FWBLACK);
    cputc(bg_color);
    cclear(38);
    cputs(get_loci_devname(presentdir[dirnr].drive, 38));

    // Get drive ID
    gotoxy(0, ypos + 1);
    cputc(A_FWBLACK);
    cputc(bg_color);
    cclear(38);
    gotoxy(2, ypos + 1);
    strncpy(buffer, presentdir[dirnr].path, 3);
    buffer[3] = 0;
    cputs(buffer);

    // Get rest of path and check if it fits else shorten
    cprintf("%.35s", presentdir[dirnr].path + 3);
}

void dir_print_entry(unsigned dirnr, unsigned char printpos)
// Print directory entry
// Input: present = pointer to directory element
//        dirnr = directory number
//        printpos = position to print
{
    unsigned char ypos = (dirnr) ? PANE2_YPOS : PANE1_YPOS;
    unsigned char bg_color, fg_color;

    // Set colors based on whether pane is active or not
    if (activepane == dirnr)
    {
        fg_color = (printpos == presentdir[dirnr].position) ? A_FWBLACK : A_FWYELLOW;
        bg_color = (printpos == presentdir[dirnr].position) ? A_BGCYAN : A_BGBLACK;
    }
    else
    {
        fg_color = A_FWWHITE;
        bg_color = A_BGBLACK;
    }

    ypos += printpos + 2;

    // Clear area
    gotoxy(0, ypos);
    cputc(fg_color);
    cputc(bg_color);
    cclear(38);
    gotoxy(2, ypos);

    // Print '-'indicator if selected
    if (presentdirelement.meta.select)
    {
        cputc('-');
    }
    else
    {
        cputc(' ');
    }

    // Print entry data
    sprintf(buffer, "%.32s", presentdirelement.name, 32);
    cprintf("%-32s %.3s", buffer, dir_entry_types[presentdirelement.meta.type - 1]);
}

void dir_draw(unsigned char dirnr, unsigned char readdir)
// Draw the dirlisting
// Input: dirnr = directory number
//        readdir = read directory, flag for 0 = just reprint same dir, 1 = read new dir
{
    unsigned char ypos = (dirnr) ? PANE2_YPOS : PANE1_YPOS;
    unsigned char printpos = 0;
    unsigned element;

    // Clear area
    cleararea(ypos, 12);

    // Print header
    dir_print_id_and_path(dirnr);

    // Read directory contents
    if (readdir)
    {
        dir_read(dirnr, filter);
        presentdir[dirnr].present = presentdir[dirnr].firstprint;
    }

    // Print no data if no valid entries in dir are found
    if (!presentdir[dirnr].firstprint)
    {
        gotoxy(0, ypos + 2);
        cputc(A_FWRED);
        cputc(A_BGBLACK);
        cputs("Empty directory.");
    }
    // Print entries until area is filled or last item is reached
    else
    {
        // Get direlement
        element = presentdir[dirnr].firstprint;

        // Loop while area is not full and further direntries are still present
        do
        {
            // Print entry and increase printpos
            dir_get_element(element);
            dir_print_entry(dirnr, printpos);
            presentdir[dirnr].lastprint = element;
            if (printpos == presentdir[dirnr].position)
            {
                presentdir[dirnr].present = element;
            }
            printpos++;

            // Check if next dir entry is present, if no: break. If yes: update present pointer
            if (!presentdirelement.meta.next)
            {
                break;
            }
            else
            {
                element = presentdirelement.meta.next;
            }

        } while (printpos < PANE_HEIGHT);
        present = presentdir[dirnr].present;
    }
}

void dir_get_next_drive(unsigned char dirnr)
// Get the next avaailable drive for pane
{
    unsigned char drive = presentdir[dirnr].drive;
    unsigned char valid = 0;

    // Find first next valid drive
    do
    {
        drive++;
        if (drive > MAXDEV)
        {
            drive = 0;
        }
        if (locicfg.validdev[drive])
        {
            valid = 1;
        }
    } while (!valid);

    // Set drive
    presentdir[dirnr].drive = drive;

    // Set root as path
    sprintf(buffer, "%1u:/", presentdir[dirnr].drive);
    strncpy(presentdir[dirnr].path, buffer, 4);

    // Draw new dir
    dir_draw(dirnr, 1);
}

void dir_get_prev_drive(unsigned char dirnr)
// Get the previous avaailable drive for pane
{
    unsigned char drive = presentdir[dirnr].drive;
    unsigned char valid = 0;

    // Find first previous valid drive
    do
    {
        if (drive > 0)
        {
            drive--;
        }
        else
        {
            drive = MAXDEV;
        }

        if (locicfg.validdev[drive])
        {
            valid = 1;
        }
    } while (!valid);

    // Set drive
    presentdir[dirnr].drive = drive;

    // Set root as path
    sprintf(buffer, "%1u:/", presentdir[dirnr].drive);
    strncpy(presentdir[dirnr].path, buffer, 4);

    // Draw new dir
    dir_draw(dirnr, 1);
}

void dir_switch_pane()
// Switch between active and non active pane
{
    activepane = !activepane;
    dir_draw(0, 0);
    dir_draw(1, 0);
}

void dir_go_down()
// Scroll down in dir of active pane
{
    // Are there dir entries? And is there a next entry?
    if (presentdir[activepane].firstelement && presentdirelement.meta.next)
    {
        present = presentdirelement.meta.next;
        dir_get_element(present);
        presentdir[activepane].present = present;
        presentdir[activepane].position++;

        // Check if next entry is on a next page. If yes, print next page
        if (presentdir[activepane].position > PANE_HEIGHT - 1)
        {
            presentdir[activepane].position = 0;
            presentdir[activepane].firstprint = present;
            dir_draw(activepane, 0);
        }
        else
        {
            // Select next entry
            previous = presentdirelement.meta.prev;
            dir_get_element(previous);
            dir_print_entry(activepane, presentdir[activepane].position - 1);
            dir_get_element(present);
            dir_print_entry(activepane, presentdir[activepane].position);
        }
    }
}

void dir_go_up()
// Scroll up in dir of active pane
{
    unsigned char count;

    // Are there dir entries? And is there a previous entry?
    if (presentdir[activepane].firstelement && presentdirelement.meta.prev)
    {
        present = presentdirelement.meta.prev;
        dir_get_element(present);
        presentdir[activepane].present = present;

        // Check if previous entry is on a previous page. If yes, print previous page
        if (!presentdir[activepane].position)
        {
            presentdir[activepane].position = PANE_HEIGHT - 1;
            for (count = 0; count < 9; count++)
            {
                if (!presentdirelement.meta.prev)
                {
                    break;
                }
                present = presentdirelement.meta.prev;
                dir_get_element(present);
            }
            presentdir[activepane].firstprint = present;
            dir_draw(activepane, 0);
        }
        else
        {
            // Select previous entry
            presentdir[activepane].position--;
            next = presentdirelement.meta.next;
            dir_get_element(next);
            dir_print_entry(activepane, presentdir[activepane].position + 1);
            dir_get_element(present);
            dir_print_entry(activepane, presentdir[activepane].position);
        }
    }
}

void dir_last_of_page()
// Go to last entry in present page
{
    unsigned element;
    unsigned char count;
    unsigned char position = 0;
    unsigned char oldpos = presentdir[activepane].position;

    // Are there dir entries?
    if (presentdir[activepane].firstelement)
    {
        // Find last of page
        element = presentdir[activepane].firstprint;
        dir_get_element(element);
        for (count = 0; count < PANE_HEIGHT - 1; count++)
        {
            if (!presentdirelement.meta.next)
            {
                break;
            }
            position++;
            element = presentdirelement.meta.next;
            dir_get_element(element);
        }

        // Set new variables and pint old and new entries in correct colour
        presentdir[activepane].position = position;
        dir_get_element(present);
        dir_print_entry(activepane, oldpos);
        presentdir[activepane].present = element;
        present = element;
        dir_get_element(present);
        dir_print_entry(activepane, position);
    }
}

void dir_pagedown()
// Page down in dir of active pane
{
    unsigned element;
    unsigned char count;

    // Are there dir entries?
    if (presentdir[activepane].firstelement)
    {
        element = presentdir[activepane].lastprint;
        dir_get_element(element);

        // Is there a next page?
        if (presentdirelement.meta.next)
        {
            dir_get_element(present);
            // Search how long next page is
            for (count = 0; count < PANE_HEIGHT; count++)
            {
                if (!presentdirelement.meta.next)
                {
                    break;
                }
                present = presentdirelement.meta.next;
                dir_get_element(present);
            }

            // Set new firstprint and present
            presentdir[activepane].firstprint = present;
            presentdir[activepane].present = present;
            presentdir[activepane].position = 0;

            // Print new page
            dir_draw(activepane, 0);
        }
        else
        {
            // Go to boyyom
            dir_last_of_page();
        }
    }
}

void dir_pageup()
// Page up in dir of active pane
{
    unsigned char count;

    // Are there dir entries?
    if (presentdir[activepane].firstelement && presentdirelement.meta.prev)
    {
        // Search how long previous page is
        for (count = 0; count < PANE_HEIGHT; count++)
        {
            if (!presentdirelement.meta.prev)
            {
                break;
            }
            present = presentdirelement.meta.prev;
            dir_get_element(present);
        }

        // Set new firstprint and present
        presentdir[activepane].firstprint = present;
        presentdir[activepane].present = present;
        presentdir[activepane].position = 0;

        // Print new page
        dir_draw(activepane, 0);
    }
}

void dir_top()
// Go to top of dir in active pane
{
    // Are there dir entries?
    if (presentdir[activepane].firstelement)
    {
        // Set present to first element and print new page
        present = presentdir[activepane].firstelement;
        dir_get_element(present);
        presentdir[activepane].present = present;
        presentdir[activepane].position = 0;
        presentdir[activepane].firstprint = present;
        dir_draw(activepane, 0);
    }
}

void dir_bottom()
// Go to bottom of dir in active pane
{
    unsigned char count;

    // Are there dir entries? And is there a next dir entry
    if (presentdir[activepane].firstelement && presentdirelement.meta.next)
    {
        // Check if not already at bottom page
        present = presentdir[activepane].lastprint;
        dir_get_element(present);
        if (!presentdirelement.meta.next)
        {
            dir_last_of_page();
            return;
        }

        // Find last element
        present = presentdir[activepane].firstelement;
        do
        {
            dir_get_element(present);
            if (presentdirelement.meta.next)
            {
                present = presentdirelement.meta.next;
            }
            else
            {
                break;
            }
        } while (1);

        // Go back one page minus one element
        presentdir[activepane].present = present;
        presentdir[activepane].position = PANE_HEIGHT - 1;
        for (count = 0; count < PANE_HEIGHT - 1; count++)
        {
            present = presentdirelement.meta.prev;
            dir_get_element(present);
        }
        presentdir[activepane].firstprint = present;
        present = presentdir[activepane].lastprint;

        // Print new page
        dir_draw(activepane, 0);
    }
}

void dir_select_toggle()
// Toggle selection of active entry
{
    if (presentdir[activepane].firstelement && presentdirelement.meta.type > 1)
    {
        presentdirelement.meta.select = !presentdirelement.meta.select;
        dir_save_element(present);
        dir_print_entry(activepane, presentdir[activepane].position);
        if (presentdirelement.meta.select)
        {
            selection++;
        }
        else
        {
            selection--;
        }
    }
}

void dir_select_all(unsigned char select)
// Select or deselect all entries in active pane
// Input: select = 1 to select, 0 to deselect
{
    unsigned element;

    if (presentdir[activepane].firstelement)
    {
        element = presentdir[activepane].firstelement;
        selection = 0;
        do
        {
            dir_get_element(element);
            if (presentdirelement.meta.type > 1)
            {
                presentdirelement.meta.select = select;
                selection++;
                dir_save_element(element);
            }
            element = presentdirelement.meta.next;
        } while (element);
        dir_draw(activepane, 0);
    }
}

void dir_select_inverse()
// Select all entries in active pane
{
    unsigned element;

    if (presentdir[activepane].firstelement)
    {
        element = presentdir[activepane].firstelement;
        selection = 0;
        do
        {
            dir_get_element(element);
            if (presentdirelement.meta.type > 1)
            {
                presentdirelement.meta.select = !presentdirelement.meta.select;
                if (presentdirelement.meta.select)
                {
                    selection++;
                }
                else
                {
                    selection--;
                }
                dir_save_element(element);
            }
            element = presentdirelement.meta.next;
        } while (element);
        dir_draw(activepane, 0);
    }
}

void dir_gotoroot()
// Go to root of active pane
{
    // Set root as path
    sprintf(buffer, "%1u:/", presentdir[activepane].drive);
    strncpy(presentdir[activepane].path, buffer, 4);

    // Draw new dir
    dir_draw(activepane, 1);
}

void dir_parentdir()
// Go to parent dir of active pane
{
    unsigned char length;
    unsigned char i;

    // Check if root dir
    length = strlen(presentdir[activepane].path);
    if (length > 3)
    {
        // Find last /
        for (i = length - 2; i > 2; i--)
        {
            if (presentdir[activepane].path[i] == '/')
            {
                break;
            }
        }

        // Set new path
        presentdir[activepane].path[i + 1] = 0;

        // Draw new dir
        dir_draw(activepane, 1);
    }
}

void dir_togglesort()
// Toggle sort order of active pane
{
    sort = !sort;
    dir_draw(0, 1);
    dir_draw(1, 1);
    if (sort)
    {
        strcpy(buffer, "On    ");
    }
    else
    {
        strcpy(buffer, "Off    ");
    }
    strncpy(pulldown_titles[0][3] + 10, buffer, PULLDOWN_MAXLENGTH - 10);
}

void dir_newdir()
// Create new directory in active pane
{
    menu_messagepopup("Not implemented yet.");
}

void dir_deletedir()
// Delete directory in active pane
{
    if (presentdir[activepane].firstelement && presentdirelement.meta.type == 1)
    {
        // Derive full target path
        strncpy(pathbuffer, presentdir[activepane].path, sizeof(pathbuffer));
        strncat(pathbuffer, presentdirelement.name, sizeof(pathbuffer) - strlen(pathbuffer));

        // Check if dir is empty
        // First: open dir
        dir = opendir(pathbuffer);
        if (!dir)
        {
            // Exit if dir can not be opened
            menu_fileerrormessage();
            return;
        }

        // Then: try to read first entry
        file = readdir(dir);
        if (file->d_name[0] != 0)
        {
            // Exit if dir is not empty
            menu_messagepopup("Directory is not empty.");
            closedir(dir);
            return;
        }
        // Close dir
        closedir(dir);

        // Confirm delete
        if (!file_confirm_message("Delete dir?", presentdirelement.name))
        {
            return;
        }

        // Delete file
        if (remove(pathbuffer) != 0)
        {
            // Error message
            menu_fileerrormessage();
        }
        else
        {
            // Redraw dir
            dir_draw(activepane, 1);
        }
    }
}