#ifndef __GENERIC_H_
#define __GENERIC_H_

// Function prototypes
void wait(unsigned int wait_cycles);
unsigned char getkey(unsigned char joyallowed);
void cleararea(unsigned char ypos, unsigned char height);
signed int textInput(unsigned char xpos, unsigned char ypos, unsigned char width, char* str, unsigned char size,unsigned char validation);

#endif // __GENERIC_H_