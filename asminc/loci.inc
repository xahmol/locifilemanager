; LOCI general defines
; Original source: rp6502.inc by Rumbledethumps

; MIA UART
MIA_READY       := $03A0 ; TX=$80 RX=$40
MIA_TX          := $03A1
MIA_RX          := $03A2

; VSYNC from PIX VGA
;MIA_VSYNC       := $03A3

; RIA XRAM portal 0
MIA_RW0         := $03A4
MIA_STEP0       := $03A5
MIA_ADDR0       := $03A6

; RIA XRAM portal 1
MIA_RW1         := $03A8
MIA_STEP1       := $03A9
MIA_ADDR1       := $03AA

; RIA OS fastcall
MIA_XSTACK      := $03AC
MIA_ERRNO       := $03AD
MIA_OP          := $03AF
;MIA_IRQ         := $03B0
;MIA_SPIN        := $03B1
MIA_SPIN        := $03B0
MIA_BUSY        := $03B2 ; Bit $80
MIA_A           := $03B4
MIA_X           := $03B6
MIA_SREG        := $03B8

; RIA OS operation numbers
MIA_OP_EXIT               := $FF
MIA_OP_ZXSTACK            := $00
MIA_OP_XREG               := $01
MIA_OP_PHI2               := $02
MIA_OP_CODEPAGE           := $03
MIA_OP_LRAND              := $04
MIA_OP_STDIN_OPT          := $05
MIA_OP_CLOCK_GETRES       := $10
MIA_OP_CLOCK_GETTIME      := $11
MIA_OP_CLOCK_SETTIME      := $12
MIA_OP_CLOCK_GETTIMEZONE  := $13
MIA_OP_OPEN               := $14
MIA_OP_CLOSE              := $15
MIA_OP_READ_XSTACK        := $16
MIA_OP_READ_XRAM          := $17
MIA_OP_WRITE_XSTACK       := $18
MIA_OP_WRITE_XRAM         := $19
MIA_OP_LSEEK              := $1A
MIA_OP_UNLINK             := $1B
MIA_OP_RENAME             := $1C
MIA_OP_OPENDIR            := $80
MIA_OP_CLOSEDIR           := $81
MIA_OP_READDIR            := $82
MIA_OP_MKDIR              := $83
MIA_OP_GETCWD             := $88
MIA_OP_MOUNT              := $90
MIA_OP_UMOUNT             := $91
MIA_OP_TAP_SEEK           := $92
MIA_OP_TAP_TELL           := $93
MIA_OP_TAP_HDR            := $94
MIA_OP_BOOT               := $A0
MIA_OP_TUNE_TMAP          := $A1
MIA_OP_TUNE_TIOR          := $A2
MIA_OP_TUNE_TIOW          := $A3
MIA_OP_TUNE_TIOD          := $A4
MIA_OP_TUNE_TADR          := $A5
MIA_OP_TUNE_SCAN          := $A6

; 6522 VIA
VIA             := $0300        ; VIA base address
VIA_PB          := VIA+$0       ; Port register B
VIA_PA1         := VIA+$1       ; Port register A
VIA_PRB         := VIA+$0       ; *** Deprecated ***
VIA_PRA         := VIA+$1       ; *** Deprecated ***
VIA_DDRB        := VIA+$2       ; Data direction register B
VIA_DDRA        := VIA+$3       ; Data direction register A
VIA_T1CL        := VIA+$4       ; Timer 1, low byte
VIA_T1CH        := VIA+$5       ; Timer 1, high byte
VIA_T1LL        := VIA+$6       ; Timer 1 latch, low byte
VIA_T1LH        := VIA+$7       ; Timer 1 latch, high byte
VIA_T2CL        := VIA+$8       ; Timer 2, low byte
VIA_T2CH        := VIA+$9       ; Timer 2, high byte
VIA_SR          := VIA+$A       ; Shift register
VIA_ACR         := VIA+$B       ; Auxiliary control register
VIA_PCR         := VIA+$C       ; Peripheral control register
VIA_IFR         := VIA+$D       ; Interrupt flag register
VIA_IER         := VIA+$E       ; Interrupt enable register
VIA_PA2         := VIA+$F       ; Port register A w/o handshake

; Values in ___oserror are the union of these FatFs errors and errno.inc
.enum
    FR_OK = 32             ; Succeeded
    FR_DISK_ERR            ; A hard error occurred in the low level disk I/O layer
    FR_INT_ERR             ; Assertion failed
    FR_NOT_READY           ; The physical drive cannot work
    FR_NO_FILE             ; Could not find the file
    FR_NO_PATH             ; Could not find the path
    FR_INVALID_NAME        ; The path name format is invalid
    FR_DENIED              ; Access denied due to prohibited access or directory full
    FR_EXIST               ; Access denied due to prohibited access
    FR_INVALID_OBJECT      ; The file/directory object is invalid
    FR_WRITE_PROTECTED     ; The physical drive is write protected
    FR_INVALID_DRIVE       ; The logical drive number is invalid
    FR_NOT_ENABLED         ; The volume has no work area
    FR_NO_FILESYSTEM       ; There is no valid FAT volume
    FR_MKFS_ABORTED        ; The f_mkfs() aborted due to any problem
    FR_TIMEOUT             ; Could not get a grant to access the volume within defined period
    FR_LOCKED              ; The operation is rejected according to the file sharing policy
    FR_NOT_ENOUGH_CORE     ; LFN working buffer could not be allocated
    FR_TOO_MANY_OPEN_FILES ; Number of open files > FF_FS_LOCK
    FR_INVALID_PARAMETER   ; Given parameter is invalid
.endenum
