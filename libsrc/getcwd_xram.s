; LOCI API CC65 library
; LOCI XRAM memcopy functions
;
; Based on:
; - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
; - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
;
; Apapted and extended by Xander Mol, 2024
;
; LOCI getcwd XRAM version
; Returns the LOCI path of the assumed boot image (ROM,DSK or TAP)
; Does not go directly to XRAM on LOCI side. Saves extra FW function

    .include    "loci.inc"

    .export         _getcwd_xram
    .import         _mia_set_ax, _mia_call_int_errno
    .import         popax
    .importzp       ptr1, ptr2

; void __fastcall__ getcwd_loci(char* buf, unsigned char length);
; ----------------------------------------------------------------------
; Function to get the current working directory to buffer in RAM
; ----------------------------------------------------------------------
; Input:
;       length          --> Maximum length of the path
; Output:
;       Path cppied to destination
; Get the parameters from stack as follows:
;       value           --> A
;       buf             --> ptr2
_getcwd_xram:
    sta     ptr1                        ;Save value from A
    lda     #255
    ldx     #0
    jsr     _mia_set_ax
    lda     #MIA_OP_GETCWD
    jsr     _mia_call_int_errno
    jsr     popax                       ;xram address
    sta     ptr2
    stx     ptr2+1
    ldy     ptr1                        ;get length
@loop:
    lda     MIA_XSTACK                  ;get byte from XSTACK
    sta     (ptr2),Y                    ;copy to RAM
    beq     @done                       ;stop when zero terminated
    dey                                 ;decrease length
    bne     @loop                       ;stop at max length
@done:
    rts
