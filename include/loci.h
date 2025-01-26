/*****************************************************************************/
/*                                                                           */
/*                                   loci.h                                  */
/*                                                                           */
/*                           LOCI by Sodiumlightbaby                         */
/*                    Derived from rp6502.h by Rumbledethumps                */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef _LOCI_H
#define _LOCI_H

#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>

/* LOCI MIA $03A0-$03BA */

struct __LOCI_MIA
{
    const unsigned char ready;
    unsigned char tx;
    const unsigned char rx;
    const unsigned char vsync;
    unsigned char rw0;
    unsigned char step0;
    unsigned int addr0;
    unsigned char rw1;
    unsigned char step1;
    unsigned int addr1;
    unsigned char xstack;
    unsigned char errno_lo;
    unsigned char errno_hi;
    unsigned char op;
    unsigned char irq;
    const unsigned char spin;
    const unsigned char busy;
    const unsigned char lda;
    unsigned char a;
    const unsigned char ldx;
    unsigned char x;
    const unsigned char rts;
    unsigned int sreg;
};
#define MIA (*(volatile struct __LOCI_MIA *)0x03A0)

#define MIA_READY_TX_BIT 0x80
#define MIA_READY_RX_BIT 0x40
#define MIA_BUSY_BIT 0x80

struct __LOCI_TAP
{
    unsigned char cmd;
    unsigned char status;
    unsigned char data;
};
#define TAP (*(volatile struct __LOCI_TAP *)0x0315)

#define TAP_CMD_PLAY 0x01
#define TAP_CMD_REC 0x02
#define TAP_CMD_REW 0x03
#define TAP_CMD_BIT 0x04
#define TAP_CMD_FFW 0x05

typedef struct __TAP_HEADER
{
    unsigned char flag_int;
    unsigned char flag_str;
    unsigned char type;
    unsigned char autorun;
    unsigned char end_addr_hi;
    unsigned char end_addr_lo;
    unsigned char start_addr_hi;
    unsigned char start_addr_lo;
    unsigned char reserved;
    unsigned char filename[16];
} tap_header_t;

/* XSTACK helpers */

void __fastcall__ mia_push_long(unsigned long val);
void __fastcall__ mia_push_int(unsigned int val);
#define mia_push_char(v) MIA.xstack = v

long mia_pop_long(void);
int mia_pop_int(void);
#define mia_pop_char() MIA.xstack

/* Set the MIA fastcall register */

void __fastcall__ mia_set_axsreg(unsigned long axsreg);
void __fastcall__ mia_set_ax(unsigned int ax);
#define mia_set_a(v) MIA.a = v

/* Run an OS operation */

int __fastcall__ mia_call_int(unsigned char op);
long __fastcall__ mia_call_long(unsigned char op);
void __fastcall__ mia_call_void(unsigned char op);

/* These run _mappederrno() on error */

int __fastcall__ mia_call_int_errno(unsigned char op);
long __fastcall__ mia_call_long_errno(unsigned char op);

/* OS operation numbers */

#define MIA_OP_EXIT 0xFF
#define MIA_OP_ZXSTACK 0x00
#define MIA_OP_XREG 0x01
#define MIA_OP_PHI2 0x02
#define MIA_OP_CODEPAGE 0x03
#define MIA_OP_LRAND 0x04
#define MIA_OP_STDIN_OPT 0x05
#define MIA_OP_CLOCK_GETRES 0x10
#define MIA_OP_CLOCK_GETTIME 0x11
#define MIA_OP_CLOCK_SETTIME 0x12
#define MIA_OP_CLOCK_GETTIMEZONE 0x13
#define MIA_OP_OPEN 0x14
#define MIA_OP_CLOSE 0x15
#define MIA_OP_READ_XSTACK 0x16
#define MIA_OP_READ_XRAM 0x17
#define MIA_OP_WRITE_XSTACK 0x18
#define MIA_OP_WRITE_XRAM 0x19
#define MIA_OP_LSEEK 0x1A
#define MIA_OP_UNLINK 0x1B
#define MIA_OP_RENAME 0x1C

#define MIA_OP_OPENDIR 0x80
#define MIA_OP_CLOSEDIR 0x81
#define MIA_OP_READDIR 0x82
#define MIA_OP_MKDIR 0x83

#define MIA_OP_GETCWD 0x88

#define MIA_OP_MOUNT 0x90
#define MIA_OP_UMOUNT 0x91
#define MIA_OP_TAP_SEEK 0x92
#define MIA_OP_TAP_TELL 0x93
#define MIA_OP_TAP_HDR 0x94

#define MIA_OP_UNAME 0x98

#define MIA_OP_BOOT 0xA0
#define MIA_OP_TUNE_TMAP 0xA1
#define MIA_OP_TUNE_TIOR 0xA2
#define MIA_OP_TUNE_TIOW 0xA3
#define MIA_OP_TUNE_TIOD 0xA4
#define MIA_OP_TUNE_TADR 0xA5
#define MIA_OP_TUNE_SCAN 0xA6

/* C API for the operating system. */

int __cdecl__ xreg(char device, char channel, unsigned char address, ...);
int phi2(void);
int codepage(void);
long lrand(void);
int __fastcall__ stdin_opt(unsigned long ctrl_bits, unsigned char str_length);
int __fastcall__ read_xstack(void *buf, unsigned count, int fildes);
int __fastcall__ read_xram(unsigned buf, unsigned count, int fildes);
int __fastcall__ write_xstack(const void *buf, unsigned count, int fildes);
int __fastcall__ write_xram(unsigned buf, unsigned count, int fildes);

int __fastcall__ getcwd_xram(unsigned buf, unsigned len);
void __fastcall__ getcwd_loci(char* buf, unsigned char len);

int __fastcall__ mount(int drive, register const char *path, register const char *filename);
int __fastcall__ umount(int drive);
long __fastcall__ tap_seek(long pos);
long __fastcall__ tap_tell(void);
long __fastcall__ tap_read_header(tap_header_t *header);

int __fastcall__ tune_tmap(unsigned char delay);
int __fastcall__ tune_tior(unsigned char delay);
int __fastcall__ tune_tiow(unsigned char delay);
int __fastcall__ tune_tiod(unsigned char delay);
int __fastcall__ tune_tadr(unsigned char delay);
void __fastcall__ tune_scan_enable(void);

void __fastcall__ mia_restore_state(void);
unsigned char __fastcall__ mia_restore_buffer_ok(void);
void __fastcall__ mia_clear_restore_buffer(void);
unsigned char __fastcall__ mia_get_vmode(void);

int __fastcall__ uname(struct utsname *buf);

/* XREG location helpers */

#define xreg_mia_keyboard(...) xreg(0, 0, 0, __VA_ARGS__)
#define xreg_mia_mouse(...) xreg(0, 0, 1, __VA_ARGS__)
// #define xreg_vga_canvas(...) xreg(1, 0, 0, __VA_ARGS__)
// #define xreg_vga_mode(...) xreg(1, 0, 1, __VA_ARGS__)

/* Values in __oserror are the union of these FatFs errors and errno.h */

typedef enum
{
    FR_OK = 32,             /* Succeeded */
    FR_DISK_ERR,            /* A hard error occurred in the low level disk I/O layer */
    FR_INT_ERR,             /* Assertion failed */
    FR_NOT_READY,           /* The physical drive cannot work */
    FR_NO_FILE,             /* Could not find the file */
    FR_NO_PATH,             /* Could not find the path */
    FR_INVALID_NAME,        /* The path name format is invalid */
    FR_DENIED,              /* Access denied due to prohibited access or directory full */
    FR_EXIST,               /* Access denied due to prohibited access */
    FR_INVALID_OBJECT,      /* The file/directory object is invalid */
    FR_WRITE_PROTECTED,     /* The physical drive is write protected */
    FR_INVALID_DRIVE,       /* The logical drive number is invalid */
    FR_NOT_ENABLED,         /* The volume has no work area */
    FR_NO_FILESYSTEM,       /* There is no valid FAT volume */
    FR_MKFS_ABORTED,        /* The f_mkfs() aborted due to any problem */
    FR_TIMEOUT,             /* Could not get a grant to access the volume within defined period */
    FR_LOCKED,              /* The operation is rejected according to the file sharing policy */
    FR_NOT_ENOUGH_CORE,     /* LFN working buffer could not be allocated */
    FR_TOO_MANY_OPEN_FILES, /* Number of open files > FF_FS_LOCK */
    FR_INVALID_PARAMETER    /* Given parameter is invalid */
} FRESULT;

typedef enum
{
    LFS_ERR_OK = 128 + 0,          // No error
    LFS_ERR_IO = 128 + 5,          // Error during device operation
    LFS_ERR_CORRUPT = 128 + 84,    // Corrupted
    LFS_ERR_NOENT = 128 + 2,       // No directory entry
    LFS_ERR_EXIST = 128 + 17,      // Entry already exists
    LFS_ERR_NOTDIR = 128 + 20,     // Entry is not a dir
    LFS_ERR_ISDIR = 128 + 21,      // Entry is a dir
    LFS_ERR_NOTEMPTY = 128 + 39,   // Dir is not empty
    LFS_ERR_BADF = 128 + 9,        // Bad file number
    LFS_ERR_FBIG = 128 + 27,       // File too large
    LFS_ERR_INVAL = 128 + 22,      // Invalid parameter
    LFS_ERR_NOSPC = 128 + 28,      // No space left on device
    LFS_ERR_NOMEM = 128 + 12,      // No more memory available
    LFS_ERR_NOATTR = 128 + 61,     // No data/attr available
    LFS_ERR_NAMETOOLONG = 128 + 36 // File name too long
} LFS_RESULT;

// Directory structure
struct DIR
{
    int fd;         /* File descriptor for directory */
    unsigned off;   /* Current byte offset in directory */
    char name[256]; /* Name passed to opendir */
};
typedef struct DIR DIR;

// File attributes bits for directory entry
#define DIR_ATTR_RDO 0x01 /* Read only */
#define DIR_ATTR_SYS 0x04 /* System files (devices) */
#define DIR_ATTR_DIR 0x10 /* Directory */

// Dir entry structure
struct dirent
{
    int d_fd;
    char d_name[64];
    unsigned char d_attrib;
    unsigned char reserved;
    unsigned long d_size;
};

#define _DE_ISREG(t) ((t) & DIR_ATTR_DIR == 0)
#define _DE_ISDIR(t) ((t) & DIR_ATTR_DIR != 0)
#define _DE_ISLBL(t) (0)
#define _DE_ISLNK(t) (0)

// Dir functions
DIR *__fastcall__ opendir(const char *name);
struct dirent *__fastcall__ readdir(DIR *dir);
int __fastcall__ closedir(DIR *dir);
long __fastcall__ telldir(DIR *dir);
void __fastcall__ seekdir(DIR *dir, long offs);
void __fastcall__ rewinddir(DIR *dir);

// Storage config
#define MAXDEV 9 // Maximum of devices

struct LOCIVERSION
{
    unsigned char major;
    unsigned char minor;
    unsigned char patch;
};

struct LOCICFGSTRUCT
{
    unsigned char devnr;
    unsigned char validdev[MAXDEV+1];
    struct utsname uname;
    struct LOCIVERSION version;
};
extern struct LOCICFGSTRUCT locicfg;

void get_locicfg();
const char *get_loci_devname(unsigned char devid, unsigned char maxlength);
unsigned char loci_check_fw(unsigned char major, unsigned char minor, unsigned char patch);

// File operations
#define COPYBUF_XRAM_ADDR 0x8000
#define COPYBUF_XRAM_SIZE 0x0800
extern const char progressBar[4];

int __fastcall__ file_save(const char *file, const void *src, unsigned int count);
int __fastcall__ file_load(const char *file, void *dst, unsigned int count);
int file_copy(const char *dst, const char *src, unsigned char prog, unsigned char progx, unsigned char progy, unsigned char progl);
int __fastcall__ file_exists(const char *file);

int __fastcall__ _sysuname(struct utsname *buf);

// XRAM MemCopy
void *__fastcall__ xram_memcpy_to(unsigned dest, const void *src, size_t count);
void *__fastcall__ xram_memcpy_from(void *dest, unsigned src, size_t count);
void *__fastcall__ xram_poke(unsigned dest, unsigned char value);
unsigned char *__fastcall__ xram_peek(unsigned src);

// Overlay RAM memcopy. To make use of RAM normally under ROM ($C000-$FFFF)
void __fastcall__ enable_overlay_ram(void);
void __fastcall__ disable_overlay_ram(void);

// IJK driver API prototypes
#define IJK_JOY_LEFT 0x02
#define IJK_JOY_RIGHT 0x01
#define IJK_JOY_UP 0x10
#define IJK_JOY_DOWN 0x08
#define IJK_JOY_FIRE 0x04
void ijk_detect(void);
void ijk_read(void);
extern unsigned char ijk_present;
extern unsigned char ijk_ljoy;
extern unsigned char ijk_rjoy;

#endif /* _LOCI_H */
