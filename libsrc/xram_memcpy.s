; LOCI API CC65 library
; LOCI XRAM memcopy functions
;
; Based on:
; - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
; - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
;
; Apapted and extended by Xander Mol, 2024

.include "loci.inc"

        .export         _xram_memcpy_to, _xram_memcpy_from, _xram_poke, _xram_peek
        .import         popax, memcpy_getparams, popptr1
        .importzp       ptr1, ptr2, ptr3

.code

; void* __fastcall__ xram_memcpy_to (unsigned dest, const void* src, size_t count);
; ----------------------------------------------------------------------
; Function to copy data from RAM to XRAM
; ----------------------------------------------------------------------
; Input:
;       dest            --> Destination in XRAM
;       src             --> Source in RAM
;       count           --> Number of bytes to copy
; Get the parameters from stack as follows:
;       size            --> ptr3
;       src             --> ptr1
;       dest            --> ptr2
;       First argument (dest) will remain on stack and is returned in a/x!
_xram_memcpy_to:
        jsr     memcpy_getparams        ; Get para,eters from stack

        lda     #1                      ; Direction is upwards     
        sta     MIA_STEP0               ; Set direction
        lda     ptr2                    ; Get low byte of XRAM destination
        sta     MIA_ADDR0+0             ; Save low byte
        lda     ptr2+1                  ; Get high byte of XRAM destination
        sta     MIA_ADDR0+1             ; Save high byte

        ldx     ptr3+1                  ; Get high byte of size
        beq     mct2                    ; Jump if zero

mct1:   .repeat 2                       ; Unroll this a bit to make it faster...
        lda     (ptr1),Y                ; Read a byte
        sta     MIA_RW0                 ; Copy to XRAM
        iny                             ; Increase loop counter
        .endrepeat
        bne     mct1                    ; End of page?
        inc     ptr1+1                  ; Increase high byte of source
        dex                             ; Next 256 byte block
        bne     mct1                    ; Repeat if any

mct2:   ldx     ptr3                    ; Get the low byte of count
        beq     mctd                    ; something to copy?

mct3:   lda     (ptr1),Y                ; Read a byte
        sta     MIA_RW0                 ; Copy to XRAM
        iny                             ; Increase loop counter
        dex                             ; Decrease bytes left counter
        bne     mct3                    ; Loop until done
        jsr     MIA_SPIN                ; Wait for completion

mctd:   jmp     popax                   ; Pop ptr and return as result

; void* __fastcall__ xram_memcpy_from (void* dest, unsigned src, size_t count);
; ----------------------------------------------------------------------
; Function to copy data from XRAM to RAM
; ----------------------------------------------------------------------
; Input:
;       dest            --> Destination in RAM
;       src             --> Source in XRAM
;       count           --> Number of bytes to copy
; Get the parameters from stack as follows:
;       size            --> ptr3
;       src             --> ptr1
;       dest            --> ptr2
;       First argument (dest) will remain on stack and is returned in a/x!
_xram_memcpy_from:
        jsr     memcpy_getparams        ; Get para,eters from stack

        lda     #1                      ; Direction is upwards     
        sta     MIA_STEP0               ; Set direction
        lda     ptr1                    ; Get low byte of XRAM source
        sta     MIA_ADDR0+0             ; Save low byte
        lda     ptr1+1                  ; Get high byte of XRAM source
        sta     MIA_ADDR0+1             ; Save high byte

        ldx     ptr3+1                  ; Get high byte of size
        beq     mcf2                    ; Jump if zero

mcf1:   .repeat 2                       ; Unroll this a bit to make it faster...
        lda     MIA_RW0                 ; Read a byte
        sta     (ptr2),Y                ; Copy to RAM
        iny                             ; Increase loop counter
        .endrepeat
        bne     mcf1                    ; End of page?
        inc     ptr2+1                  ; Increase high byte of destination
        dex                             ; Next 256 byte block
        bne     mcf1                    ; Repeat if any

mcf2:   ldx     ptr3                    ; Get the low byte of count
        beq     mcfd                    ; something to copy?

mcf3:   lda     MIA_RW0                 ; Read a byte
        sta     (ptr2),Y                ; Copy to RAM
        iny                             ; Increase loop counter
        dex                             ; Decrease bytes left counter
        bne     mcf3                    ; Loop until done

mcfd:   jmp     popax                   ; Pop ptr and return as result

; void* __fastcall__ xram_poke (unsigned dest, unsigned char value);
; ----------------------------------------------------------------------
; Function to write a single byte to XRAM
; ----------------------------------------------------------------------
; Input:
;       dest            --> Destination in XRAM
;       value           --> Value to write
; Get the parameters from stack as follows:
;       value           --> A
;       dest            --> ptr1
_xram_poke:
        sta     ptr2                    ; Save value from A
        jsr     popptr1                 ; save src to ptr1
        lda     #1                      ; Direction is upwards     
        sta     MIA_STEP0               ; Set direction
        lda     ptr1                    ; Get low byte of XRAM destination
        sta     MIA_ADDR0+0             ; Save low byte
        lda     ptr1+1                  ; Get high byte of XRAM destination
        sta     MIA_ADDR0+1             ; Save high byte
        lda     ptr2                    ; Get value
        sta     MIA_RW0                 ; Store to XRAM
        jsr     MIA_SPIN                ; Wait for completion
        lda     #0                      ; Clear A
        ldx     #0                      ; Clear X
        ldy     #0                      ; Clear Y
        rts                             ; Done

; unsigned char* __fastcall__ xram_peek (unsigned src);
; ----------------------------------------------------------------------
; Function to read a single byte from XRAM
; ----------------------------------------------------------------------
; Input:
;       src             --> Source in XRAM
; Output:
;       A               --> Value read
; Get the parameters from stack as follows:
;       src             --> A/X
_xram_peek:
        tay                             ; Save value from A to Y
        lda     #1                      ; Direction is upwards     
        sta     MIA_STEP0               ; Set direction
        sty     MIA_ADDR0+0             ; Save low byte
        stx     MIA_ADDR0+1             ; Save high byte
        lda     MIA_RW0                 ; Read a byte
        ldx     #0                      ; Clear X
        ldy     #0                      ; Clear Y
        rts