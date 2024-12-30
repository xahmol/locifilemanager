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

// Variables
struct DirElement presentdirelement;
struct Directory presentdir[2];
DIR *dir;
struct dirent *file;
unsigned char activepane;
unsigned char filter;
char *diraddress[2] = {(char *)DIR1BASE, (char *)DIR2BASE};

char dir_entry_types[6][4] =
    {
        "DIR",
        "DSK",
        "TAP",
        "ROM",
        "LCE",
        "   "};

// Function prototypes
void cleararea(unsigned char ypos, unsigned char height)
//  Clear an area of the screen
//  Input: ypos = startline of area
//         height = height of area
{
    unsigned char y;
    for (y = 0; y < height; y++)
    {
        gotoxy(0, ypos + y);
        cputc(A_FWWHITE);
        cputc(A_BGBLACK);
        cclear(38);
    }
}

void dir_get_element(char *address)
// Get directory element from address
// Input: address = address of element
{
    char *workaddress = address;
    xram_memcpy_from(&presentdirelement.meta, workaddress, sizeof(presentdirelement.meta));
    workaddress += sizeof(presentdirelement.meta);
    xram_memcpy_from(presentdirelement.name, workaddress, presentdirelement.meta.length);
}

void dir_read(unsigned char dirnr, unsigned char filter)
// Read directory entries to memory from present path
// Input: dirnr = directory number
//        filter = filter for file type
{
    char *previous = 0;
    char *present;
    unsigned char presenttype;
    unsigned char datalength, count;
    unsigned char ypos = (dirnr) ? PANE2_YPOS : PANE1_YPOS;

    // Debug
    // unsigned char i;

    // Reset dir address
    diraddress[dirnr] = (dirnr) ? (char *)DIR2BASE : (char *)DIR1BASE;
    present = diraddress[dirnr];

    // Clear directory values
    presentdir[dirnr].firstelement = 0;
    presentdir[dirnr].firstprint = 0;
    presentdir[dirnr].position = 0;

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
        cputcxy(39, ypos, progress_str[0x03 & count++]);

        // Check if entry is a dir by checking if bit 4 of first byte is set
        if (file->d_attrib & DIR_ATTR_DIR)
        {
            presenttype = 1;

            // Add / to dir name if room is left
            if (datalength < 64)
            {
                file->d_name[datalength++] = '/';
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

        // Filter out non-matching file types if filter is set
        if (filter && presenttype > 1 && presenttype != (filter - 1))
        {
            presenttype = 0;
        }

        // Debug
        // cprintf("Type: %u\n\r", presenttype);

        if (presenttype)
        {
            // Check if sufficent memory is left
            if (diraddress[dirnr] + datalength + sizeof(presentdirelement.meta) > diraddress[dirnr] + DIRSIZE - 1)
            {
                return;
            }

            // Set direntry pointers
            if (!previous)
            {
                presentdir[dirnr].firstelement = present;
                presentdir[dirnr].firstprint = present;
                presentdirelement.meta.prev = 0;
            }
            else
            {
                presentdirelement.meta.prev = previous;              // Set prev in new entry
                xram_memcpy_to(previous, &present, sizeof(present)); // Set next in previous entry

                // Debug
                // cprintf("Dump previous:\n\r");
                // for (i = 0; i < (present - previous); i++)
                //{
                //    cprintf("%2x", xram_peek(previous + i));
                //}
            }

            // Debug
            // cprintf("\n\rPrev: %4x Pres: %4x ", previous, present);

            previous = present;
            presentdirelement.meta.next = 0;
            presentdirelement.meta.select = 0;
            presentdirelement.meta.type = presenttype;
            datalength++;
            presentdirelement.meta.length = datalength;

            // Set meta data
            xram_memcpy_to(diraddress[dirnr], &presentdirelement.meta, sizeof(presentdirelement.meta));
            diraddress[dirnr] += sizeof(presentdirelement.meta);

            // Set filename
            xram_memcpy_to(diraddress[dirnr], file->d_name, datalength);
            diraddress[dirnr] += datalength;

            // Update present pointer
            present = diraddress[dirnr];

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
    present = presentdir[dirnr].firstelement;
    dir_get_element(present);
    cputcxy(39, ypos, CH_SPACE);
}

void dir_print_id_and_path(unsigned char dirnr)
// Draw drive ID and path
// Input: dirnr = directory number
{
    unsigned char ypos = (dirnr) ? PANE2_YPOS : PANE1_YPOS;
    unsigned char length;
    unsigned char bg_color;

    // Set colors based on whether pane is active or not
    bg_color = (activepane == dirnr) ? A_BGYELLOW : A_BGWHITE;


    // Print device name
    gotoxy(0, ypos);
    cputc(A_FWBLACK);
    cputc(bg_color);
    cclear(38);
    cputs(get_loci_devname(presentdir[dirnr].drive,38));

    // Get drive ID
    gotoxy(0, ypos+1);
    cputc(A_FWBLACK);
    cputc(bg_color);
    cclear(38);
    gotoxy(2, ypos+1);
    strncpy(buffer, presentdir[dirnr].path, 3);
    buffer[3] = 0;
    cputs(buffer);

    // Get rest of path
    strcpy(pathbuffer, presentdir[dirnr].path + 3);

    // Check if it fits else shorten
    length = strlen(pathbuffer);
    if (length > 35)
    {
        strcpy(buffer, pathbuffer + length - 35);
    }
    else
    {
        strcpy(buffer, pathbuffer);
    }
    cputs(buffer);
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

    // Print '-'indicator if selected
    if (presentdirelement.meta.select)
    {
        cputc('-');
    }
    else
    {
        cputc(' ');
    }

    // Truncate filename if too long
    if (presentdirelement.meta.length > 32)
    {
        strncpy(pathbuffer, presentdirelement.name, 32);
        pathbuffer[32] = 0;
    }
    else
    {
        strcpy(pathbuffer, presentdirelement.name);
    }

    // Print entry data
    sprintf(buffer, "%-32s %s", pathbuffer, dir_entry_types[presentdirelement.meta.type - 1]);
    cputs(buffer);
}

void dir_draw(unsigned char dirnr, unsigned char readdir)
// Draw the dirlisting
// Input: dirnr = directory number
//        readdir = read directory, flag for 0 = just reprint same dir, 1 = read new dir
{
    unsigned char ypos = (dirnr) ? PANE2_YPOS : PANE1_YPOS;
    unsigned char printpos = 0;
    char *present;

    // Clear area
    cleararea(ypos, 13);

    // Read directory contents
    if (readdir)
    {
        dir_print_id_and_path(dirnr);
        dir_read(dirnr, filter);
    }

    // Print no data if no valid entries in dir are found
    if (!presentdir[dirnr].firstprint)
    {
        gotoxy(0, ypos + 2);
        cputc(A_FWRED);
        cputc(A_BGBLACK);
        cputs("No data!");
    }
    // Print entries until area is filled or last item is reached
    else
    {
        // Get direlement
        present = presentdir[dirnr].firstprint;

        // Loop while area is not full and further direntries are still present
        do
        {
            // Print entry and increase printpos
            dir_get_element(present);
            dir_print_entry(dirnr, printpos);
            printpos++;
            presentdir[dirnr].lastprint = present;

            // Check if next dir entry is present, if no: break. If yes: update present pointer
            if (!presentdirelement.meta.next)
            {
                break;
            }
            else
            {
                present = presentdirelement.meta.next;
            }

        } while (printpos < 10);
    }

    present = presentdir[dirnr].firstprint;
    dir_get_element(present);
}