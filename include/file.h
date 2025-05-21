#ifndef __FILE_H_
#define __FILE_H_

// Function prototypes
unsigned char file_confirm_message(const char *message, const char *file);
void file_copy_move_selected(char move);
void file_delete();
void file_rename();
void file_browse_tape();

#endif // __FILE_H_