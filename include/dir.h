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

// Structure for directory meta data
struct DirMeta
{
    unsigned next;        // Pointer to next element
    unsigned prev;        // Pointer to previous element
    unsigned char type;   // Type: 1=dir, 2=DSK, 3=TAP, 4=ROM, 5=LCE
    unsigned char select; // Select: 0=not selected, 1=selected
    unsigned char length; // Length of name
};

// Structure for directory element
struct DirElement
{
    char name[64];       // Entry name
    struct DirMeta meta; // Meta data
};
extern struct DirElement presentdirelement;

// Structure for active directory for both panes
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

// Directory reading variables
extern DIR *dir;
extern struct dirent *file;
extern char dir_entry_types[8][4]; // Directory entry type name text strings

// Application variables
extern unsigned char activepane;    // Number of active pane: 0 is upper, 1 is lower
extern unsigned present;            // Present element
extern unsigned previous;           // Previous element
extern unsigned next;               // Next element
extern unsigned char filter;        // Filter for file type, 0: None, 1: DSK, 2: TAP, 3: ROM, 4: LCE
extern unsigned char enterchoice;   // Choice for enter action: 0: Select, 1: Mount or 2: Launch
extern unsigned char confirm;       // Confirm once (01) or all (1)
extern unsigned char sort;          // Sort on (1) or off (0)
extern unsigned char targetdrive;   // Target drive for mount: 0: A, 1: B, 2: C, 3: D
extern int selection;               // Selection present for file operation: 0: not present, 1: present
extern unsigned char insidetape[2]; // Browser is inside a tape .TAP container file

// Buffers for full paths
extern char pathbuffer[256];
extern char pathbuffer2[256];

// Function prototypes
void dir_get_element(unsigned address);
void dir_save_element(unsigned address);
void dir_read(unsigned char dirnr, unsigned char filter);
void dir_tape_parse(unsigned char dirnr);
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
void dir_newdir();
void dir_deletedir();

#endif // __DIR_H_