; LOCI API CC65 library
; LOCI XRAM memcopy functions
;
; Based on:
; - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
; - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
;
; Apapted and extended by Xander Mol, 2024
;
; Sodiumlightbaby 2024
;
; LOCI getcwd XRAM version
; Returns the LOCI path of the assumed boot image (ROM,DSK or TAP)
; Does not go directly to XRAM on LOCI side. Saves extra FW function

    .include    "loci.inc"

    .export         _getcwd_xram
    .import         _mia_set_ax, _mia_call_int_errno
    .import         popax
    .importzp       ptr1, ptr2

; void __fastcall__ getcwd_xram(char* address, unsigned char len);
; ----------------------------------------------------------------------
; Function to copy LOCI current working directory to XRAM
; ----------------------------------------------------------------------
; Input:
;       address         --> Destination in XRAM
;       len             --> Maximum length
; Get the parameters from stack as follows:
;       len             --> ptr1
;       address         --> A/X
_getcwd_xram:
    sta     ptr1                    ;max lenght
    stx     ptr1+1
    lda     #255
    ldx     #0
    jsr     _mia_set_ax
    lda     #MIA_OP_GETCWD
    jsr     _mia_call_int_errno
    jsr     popax                   ;xram address
    sta     MIA_ADDR0
    stx     MIA_ADDR0+1
    lda     #1
    sta     MIA_STEP0
    ldy     ptr1                    ;TODO Assuming max 255 chars
@loop:
    lda     MIA_XSTACK
    sta     MIA_RW0
    beq     @done                   ;stop when zero terminated
    dey
    bne     @loop                   ;stop at max length
@done:
    rts
