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
#include "drive.h"

// Variables
char mount_filename[6][21];

// Functions
void drive_unmount_all()
// Unmount all images for disk, tape and ROM
{
    unsigned char drive;

    for (drive = 0; drive < 6; drive++)
    {
        umount(drive);
        mount_filename[drive][0] = 0;
    }
}

void drive_showmounts()
// Function to show filenames that are mounted
{
    unsigned char drive;

    windownew(2, 8, 12);

    cputsxy(4, 9, "Present mounts:");

    // Disc drives
    for (drive = 0; drive < 4; drive++)
    {
        gotoxy(4, 11 + drive);
        cprintf("Drive %c: %.20s", 'A' + drive, mount_filename[drive]);
    }

    // Cassette
    gotoxy(4, 15);
    cprintf("Tape   : %.20s", mount_filename[4]);

    // ROM
    gotoxy(4, 16);
    cprintf("ROM    : %.20s", mount_filename[5]);

    cputsxy(4, 18, "Press key.");
    getkey(ijk_present);

    windowrestore();
}

void drive_mount()
// Mount selected file
{
    strcpy(buffer, "Mount did not succeed.");

    // Are there any files in dir?
    if (presentdir[activepane].firstelement && presentdirelement.meta.type < 5)
    {
        // .DSK file
        if (presentdirelement.meta.type == 2)
        {
            if (!mount(targetdrive, presentdir[activepane].path, presentdirelement.name))
            {
                sprintf(buffer, "%.20s on %c.", presentdirelement.name, 'A' + targetdrive);
                strncpy(mount_filename[targetdrive],presentdirelement.name,21);
            }
        }

        // .TAP file
        if (presentdirelement.meta.type == 3)
        {
            if (!mount(4, presentdir[activepane].path, presentdirelement.name))
            {
                sprintf(buffer, "%.20s for tape.",presentdirelement.name);
                strncpy(mount_filename[4],presentdirelement.name,21);
            }
        }

        // .ROM file
        if (presentdirelement.meta.type == 4)
        {
            if (!mount(5, presentdir[activepane].path, presentdirelement.name))
            {
                sprintf(buffer, "%.20s for ROM.",presentdirelement.name);
                strncpy(mount_filename[5],presentdirelement.name,21);
            }
        }

        // Show message
        menu_messagepopup(buffer);
    }
}