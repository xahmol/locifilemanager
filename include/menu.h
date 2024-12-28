#ifndef __MENU_H_
#define __MENU_H_

#define WINDOWBASEADDRESS   0xC000      // Base address for windows system data
#define WINDOWBUFSIZE       0x0800      // Size of window buffer

//Window data
struct WindowStruct
{
    unsigned int address;
    unsigned char ypos;
    unsigned char height;
};
extern struct WindowStruct Window[9];
extern unsigned int windowaddress;
extern unsigned char windownumber;

//Menu data
extern unsigned char menubaroptions;
extern unsigned char pulldownmenunumber;
extern char menubartitles[4][12];
extern unsigned char menubarcoords[4];
extern unsigned char pulldownmenuoptions[5];
extern char pulldownmenutitles[5][6][16];

#endif // __MENU_H_