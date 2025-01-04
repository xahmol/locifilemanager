#ifndef __DIR_H_
#define __DIR_H_

// Defines
// Base addresses for dir entries in XRAM
#define DIR1BASE COPYBUF_XRAM_ADDR + COPYBUF_XRAM_SIZE // Base address for dir 1
#define DIRSIZE 0x0C00                                 // Room for directory entries
#define DIR2BASE (DIR1BASE + DIRSIZE)                  // Base address for dir 2

// Pane location and height
#define PANE1_YPOS 3
#define PANE2_YPOS 15
#define PANE_HEIGHT 10

// Structs and variables
struct DirMeta
{
    unsigned next;        // Pointer to next element
    unsigned prev;        // Pointer to previous element
    unsigned char type;   // Type: 1=dir, 2=DSK, 3=TAP, 4=ROM, 5=LCE
    unsigned char select; // Select: 0=not selected, 1=selected
    unsigned char length; // Length of name
};

struct DirElement
{
    char name[64];       // Entry name
    struct DirMeta meta; // Meta data
};
extern struct DirElement presentdirelement;

struct Directory
{
    unsigned firstelement;  // Pointer to first element
    unsigned firstprint;    // Pointer to first element to print
    unsigned lastprint;     // Pointer to last element to print
    unsigned present;       // Pointer to active element
    unsigned char drive;    // Drive number
    unsigned char position; // Position in directory
    char path[256];         // Path
    unsigned address;       // Address in XRAM for next entry
};
extern struct Directory presentdir[2];

extern DIR *dir;
extern struct dirent *file;

extern char dir_entry_types[6][4];
extern unsigned char activepane;
extern unsigned present;
extern unsigned previous;
extern unsigned next;
extern unsigned char filter;
extern unsigned char enterchoice;
extern unsigned char confirm;
extern unsigned char sort;
extern char pathbuffer[256];
extern char pathbuffer2[256];

// Function prototypes
void cleararea(unsigned char ypos, unsigned char height);
char *truncate(const char *src, unsigned char length);
void dir_get_element(unsigned address);
void dir_save_element(unsigned address);
void dir_read(unsigned char dirnr, unsigned char filter);
void dir_print_id_and_path(unsigned char dirnr);
void dir_print_entry(unsigned dirnr, unsigned char printpos);
void dir_draw(unsigned char dirnr, unsigned char readdir);
void dir_get_next_drive(unsigned char dirnr);
void dir_get_prev_drive(unsigned char dirnr);
void dir_switch_pane();
void dir_go_down();
void dir_go_up();
void dir_pagedown();
void dir_pageup();
void dir_top();
void dir_bottom();
void dir_last_of_page();
void dir_select_toggle();
void dir_select_all(unsigned char select);
void dir_select_inverse();
void dir_gotoroot();
void dir_parentdir();
void dir_togglesort();

#endif // __DIR_H_