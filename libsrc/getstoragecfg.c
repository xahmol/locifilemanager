// LOCI API CC65 library
//
// Get storage configuration functions
//
// Based on:
// - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
// - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
// - cc65 by Ullrich von Bassewitz,  Based on code by Groepaz. https://github.com/cc65/cc65
//
// Apapted and extended by Xander Mol, 2024

#include <loci.h>
#include <string.h>
#include <conio.h>

struct LOCICFGSTRUCT locicfg;

void get_locicfg()
// Function to get the LOCI configuration
// Output:  locicfg - structure with LOCI configuration
//          locicfg.devnr - number of devices
//          locicfg.validdev - array with valid devices: 0 - not valid, 1 - valid
//          locicfg.path - path of the current device
{
    unsigned char devid;

    DIR *dir;
    struct dirent *fil;

    // Wipe config
    memset(&locicfg, 0, sizeof(locicfg));

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