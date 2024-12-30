#ifndef __DIR_H_
#define __DIR_H_

// Defines
// Base addresses for dir entries in XRAM
#define DIR1BASE FM_XRAM_ADDR            // Base address for dir 1
#define DIR2BASE (FM_XRAM_ADDR + 0x1000) // Base address for dir 2
#define DIRSIZE 0x1000                   // Room for directory entries

// Pane location
#define PANE1_YPOS 3
#define PANE2_YPOS 15

// Structs and variables
struct DirMeta
{
    char *next;           // Pointer to next element
    char *prev;           // Pointer to previous element
    unsigned char type;   // Type: 1=dir, 2=DSK, 3=TAP, 4=ROM, 5=LCE
    unsigned char select; // Select: 0=not selected, 1=selected
    unsigned char length; // Length of name
};

struct DirElement
{
    char name[64];         // Entry name
    struct DirMeta meta;   // Meta data
};
extern struct DirElement presentdirelement;

struct Directory
{
    char *firstelement;     // Pointer to first element
    char *lastelement;      // Pointer to last element
    char *firstprint;       // Pointer to first element to print
    char *lastprint;        // Pointer to last element to print
    unsigned char drive;    // Drive number
    unsigned char position; // Position in directory
    char path[256];         // Path
};
extern struct Directory presentdir[2];

extern DIR *dir;
extern struct dirent *file;

extern char *diraddress[2];
extern char dir_entry_types[6][4];
extern unsigned char activepane;
extern unsigned char filter;

// Function prototypes
void cleararea(unsigned char ypos, unsigned char height);
void dir_get_element(char *address);
void dir_read(unsigned char dirnr, unsigned char filter);
void dir_print_id_and_path(unsigned char dirnr);
void dir_print_entry(unsigned dirnr, unsigned char printpos);
void dir_draw(unsigned char dirnr, unsigned char readdir);

#endif // __DIR_H_