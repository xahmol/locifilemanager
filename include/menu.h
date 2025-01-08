#ifndef __MENU_H_
#define __MENU_H_

// Define
// Set windowsing system address space
#define WINDOWBASEADDRESS 0xC000 // Base address for windows system data
#define WINDOWBUFSIZE 0x0800     // Size of window buffer

// Set these for menubar and pulldown menu dimensions
#define MENUBAR_MAXOPTIONS 5  // Number of options in main menu bar
#define MENUBAR_MAXLENGTH 12  // Maxiumum length of main menu bar option names
#define PULLDOWN_NUMBER 9    // Number of pull down menus defined
#define PULLDOWN_MAXOPTIONS 7 // Maximum number of options for pull down meus
#define PULLDOWN_MAXLENGTH 17 // Maximum length of pull down menu option names
#define MENU_YESNO 8          // Number for Yes No pull down menu

// Function prototypes
// Windows
void windowsave(unsigned char ypos, unsigned char height);
void windowrestore();
void windownew(unsigned char xpos, unsigned char ypos, unsigned char height);

// Menus
void menu_placeheader(const char *header);
void menu_placebar(unsigned char y);
void menu_placetop(const char *header);
unsigned char menu_pulldown(unsigned char xpos, unsigned char ypos, unsigned char menunumber, unsigned char escapable);
unsigned char menu_main();
unsigned char menu_areyousure(const char *message);
void menu_fileerrormessage();
void menu_messagepopup(const char *message);

// Variables
// Windows
struct WindowStruct
{
    char* address;
    unsigned char ypos;
    unsigned char height;
};
extern struct WindowStruct Window[9];
extern char* windowaddress;
extern unsigned char windownumber;

// Menus
struct MenuBar
{
    char titles[MENUBAR_MAXOPTIONS][MENUBAR_MAXLENGTH];
    unsigned char xstart[MENUBAR_MAXOPTIONS];
    unsigned char ypos;
};
extern struct MenuBar menubar;
extern char pulldown_options[PULLDOWN_NUMBER];
extern char pulldown_titles[PULLDOWN_NUMBER][PULLDOWN_MAXOPTIONS][PULLDOWN_MAXLENGTH];

#endif // __MENU_H_