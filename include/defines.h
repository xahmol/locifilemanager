#ifndef __DEFINES_H_
#define __DEFINES_H_

// Global variables
extern char buffer[41];
extern char version[22];
extern unsigned char fdc_on;
extern unsigned char tap_on;
extern unsigned char b11_on;
extern unsigned char bit_on;
extern unsigned char ald_on;


/* Char defines */
#define CH_SPACE            0x20        // Screencode for space
#define CH_INVSPACE         0xA0        // Inverse space
#define CH_MINUS            0x2D        // Screencode for minus
#define CH_CTRL_A           0x01        // Control+A
#define CH_CTRL_B           0x02        // Control+B
#define CH_CTRL_C           0x03        // Control+C
#define CH_CTRL_D           0x04        // Control+D
#define CH_CTRL_E           0x05        // Control+E
#define CH_CTRL_F           0x06        // Control+F
#define CH_CTRL_G           0x07        // Control+G
#define CH_CTRL_H           0x08        // Control+H
#define CH_CTRL_I           0x09        // Control+I
#define CH_CTRL_J           0x0A        // Control+J
#define CH_CTRL_K           0x0B        // Control+K
#define CH_CTRL_L           0x0C        // Control+L
#define CH_CTRL_M           0x0D        // Control+M
#define CH_CTRL_N           0x0E        // Control+N
#define CH_CTRL_O           0x0F        // Control+O
#define CH_CTRL_P           0x10        // Control+P
#define CH_CTRL_Q           0x11        // Control+Q
#define CH_CTRL_R           0x12        // Control+R
#define CH_CTRL_S           0x13        // Control+S
#define CH_CTRL_T           0x14        // Control+T
#define CH_CTRL_U           0x15        // Control+U
#define CH_CTRL_V           0x16        // Control+V
#define CH_CTRL_W           0x17        // Control+W
#define CH_CTRL_X           0x18        // Control+X
#define CH_CTRL_Y           0x19        // Control+Y
#define CH_CTRL_Z           0x1A        // Control+Z

/* Defines for versioning */
/* Version number */
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 2
/* Build year */
#define BUILD_YEAR_CH0 (__DATE__[ 7])
#define BUILD_YEAR_CH1 (__DATE__[ 8])
#define BUILD_YEAR_CH2 (__DATE__[ 9])
#define BUILD_YEAR_CH3 (__DATE__[10])
/* Build month */
#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')
#define BUILD_MONTH_CH0 \
    ((BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC) ? '1' : '0')
#define BUILD_MONTH_CH1 \
    ( \
        (BUILD_MONTH_IS_JAN) ? '1' : \
        (BUILD_MONTH_IS_FEB) ? '2' : \
        (BUILD_MONTH_IS_MAR) ? '3' : \
        (BUILD_MONTH_IS_APR) ? '4' : \
        (BUILD_MONTH_IS_MAY) ? '5' : \
        (BUILD_MONTH_IS_JUN) ? '6' : \
        (BUILD_MONTH_IS_JUL) ? '7' : \
        (BUILD_MONTH_IS_AUG) ? '8' : \
        (BUILD_MONTH_IS_SEP) ? '9' : \
        (BUILD_MONTH_IS_OCT) ? '0' : \
        (BUILD_MONTH_IS_NOV) ? '1' : \
        (BUILD_MONTH_IS_DEC) ? '2' : \
        /* error default */    '?' \
    )
/* Build day */
#define BUILD_DAY_CH0 ((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define BUILD_DAY_CH1 (__DATE__[ 5])
/* Build hour */
#define BUILD_HOUR_CH0 (__TIME__[0])
#define BUILD_HOUR_CH1 (__TIME__[1])
/* Build minute */
#define BUILD_MIN_CH0 (__TIME__[3])
#define BUILD_MIN_CH1 (__TIME__[4])

#endif // __DEFINES_H_