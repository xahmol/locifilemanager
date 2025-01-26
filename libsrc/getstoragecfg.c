// LOCI API CC65 library
//
// Get storage configuration functions
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Adapted and extended by Xander Mol, 2025
#include <stdlib.h>
#include <loci.h>
#include <string.h>
#include <conio.h>
#include <osdklib.h>

struct LOCICFGSTRUCT locicfg;

void get_locicfg()
// Function to get the LOCI configuration
// Output:
//  -   Exits program with error message if no LOCI detected
//  -   Else fills locicfg - structure with LOCI configuration
//      -   locicfg.devnr - number of devices
//      -   locicfg.validdev - array with valid devices: 0 - not valid, 1 - valid
//      -   locicfg.uname - uname structure with Loci device data
{
    unsigned char devid;

    DIR *dir;
    struct dirent *fil;

    // Detect if Loci is present
    if (peek(0x0319) != 'L')
    {
        cprintf("No LOCI device detected or FW to old.");
        cprintf("\n\rPress key to exit\n\r");
        cgetc();
        clrscr();
        setflags(SCREEN);
        exit(1);
    }

    // Wipe config
    memset(&locicfg, 0, sizeof(locicfg));

    // Get uname data
    _sysuname(&locicfg.uname);

    // Set version number struct
    locicfg.version.major = locicfg.uname.release[0] - '0';
    locicfg.version.minor = locicfg.uname.release[2] - '0';
    locicfg.version.patch = locicfg.uname.release[4] - '0';
    if (locicfg.uname.release[5])
    {
        locicfg.version.patch = locicfg.version.patch * 10 + locicfg.uname.release[5] - '0';
    }

    // Set drive 0 as valid as it is always valid
    locicfg.validdev[0] = 1;

    // Get dir of root dir to parse devices
    dir = opendir("");

    while (fil = readdir(dir), fil->d_name[0] != 0 && locicfg.devnr < MAXDEV + 1)
    {
        devid = fil->d_name[0] - '0'; // Get device number

        if (devid)
        {
            locicfg.devnr++;

            // Check for MSC to know if it is a USB mass storage device
            if (fil->d_name[3] == 'M' && fil->d_name[4] == 'S' && fil->d_name[5] == 'C')
            {
                locicfg.validdev[devid] = 1;
            }
        }
    }
    closedir(dir);
}

const char *get_loci_devname(unsigned char devid, unsigned char maxlength)
// Function to get the name of a device
// Input:   devid - device number
//          maxlength - maximum length of the name
// Output:  name of the device
{
    DIR *dir;
    struct dirent *fil;
    unsigned char i;

    dir = opendir("");
    for (i = 0; i < devid + 1; i++)
    {
        fil = readdir(dir);
    }
    closedir(dir);
    if (maxlength < strlen(fil->d_name + 3))
    {
        fil->d_name[maxlength + 3] = 0;
    }
    return fil->d_name + 3;
}

unsigned char loci_check_fw(unsigned char major, unsigned char minor, unsigned char patch)
// Function to check if the LOCI firmware is at least a certain version
// Input:   major - minimum major version
//          minor - minimum minor version
//          patch - minimum patch version
// Output:  1 - firmware is at least the minimum version
//          0 - firmware is older than the minimum version
{
    if (locicfg.version.major > major)
    {
        return 1;
    }
    if (locicfg.version.major == major && locicfg.version.minor > minor)
    {
        return 1;
    }
    if (locicfg.version.major == major && locicfg.version.minor == minor && locicfg.version.patch >= patch)
    {
        return 1;
    }
    return 0;
}