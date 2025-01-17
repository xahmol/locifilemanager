#ifndef __DRIVE_H_
#define __DRIVE_H_

// Variables
extern char mount_filename[6][21];

// Function prototypes
void drive_targetdrive();
void drive_unmount_all();
void drive_showmounts();
void drive_mount();
void drive_unmount();

#endif // __DRIVE_H_