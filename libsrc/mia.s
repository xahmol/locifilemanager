; LOCI API CC65 library
; Helpers for building API shims
;
; Based on:
; - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
; - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
;
; Apapted and extended by Xander Mol, 2025

.include "loci.inc"
.include "errno.inc"
.export _mia_push_long, _mia_push_int
.export _mia_pop_long, _mia_pop_int
.export _mia_set_axsreg, _mia_set_ax
.export _mia_call_int, _mia_call_long
.export _mia_call_int_errno, _mia_call_long_errno
.export _mia_call_void

.importzp sp, sreg
.import incsp1

.code

; void __fastcall__ mia_push_long(unsigned long val);
; ----------------------------------------------------------------------
; Function to push a long value to the stack
; ----------------------------------------------------------------------
; Input:
;       val             --> Value to push
; ---------------------------------------------------------------------- 
_mia_push_long:
    ldy sreg+1
    sty MIA_XSTACK
    ldy sreg
    sty MIA_XSTACK

; void __fastcall__ mia_push_int(unsigned int val);
; ----------------------------------------------------------------------
; Function to push an int value to the stack
; ----------------------------------------------------------------------
; Input:
;       val             --> Value to push
; ----------------------------------------------------------------------
_mia_push_int:
    stx MIA_XSTACK
    sta MIA_XSTACK
    rts

; long __fastcall__ mia_pop_long(void);
; ----------------------------------------------------------------------
; Function to pop a long value from the stack
; ----------------------------------------------------------------------
; Output:                --> Popped value
; ----------------------------------------------------------------------
_mia_pop_long:
    jsr _mia_pop_int
    ldy MIA_XSTACK
    sty sreg
    ldy MIA_XSTACK
    sty sreg+1
    rts

; int __fastcall__ mia_pop_int(void);
; ----------------------------------------------------------------------
; Function to pop an int value from the stack
; ----------------------------------------------------------------------
; Output:                --> Popped value
; ----------------------------------------------------------------------
_mia_pop_int:
    lda MIA_XSTACK
    ldx MIA_XSTACK
    rts

; void __fastcall__ mia_set_axsreg(unsigned long axsreg);
; ----------------------------------------------------------------------
; Function to set the MIA register
; ----------------------------------------------------------------------
; Input:            --> Value to set
; ----------------------------------------------------------------------
_mia_set_axsreg:
    ldy sreg
    sty MIA_SREG
    ldy sreg+1
    sty MIA_SREG+1

; void __fastcall__ mia_set_ax(unsigned int ax);
; ----------------------------------------------------------------------
; Function to set the value for the MIA register
; ----------------------------------------------------------------------
; Input:            --> Value to set
; ----------------------------------------------------------------------
_mia_set_ax:
    stx MIA_X
    sta MIA_A
    rts

; int __fastcall__ mia_call_int(unsigned char op);
; ----------------------------------------------------------------------
; Function to call a MIA function that returns an int
; ----------------------------------------------------------------------
; Input:        --> Operation code
; Output:       --> Result
; ----------------------------------------------------------------------
_mia_call_int:
    sta MIA_OP
    jmp MIA_SPIN

; long __fastcall__ mia_call_long(unsigned char op);
; ----------------------------------------------------------------------
; Function to call a MIA function that returns a long
; ----------------------------------------------------------------------
; Input:        --> Operation code
; Output:       --> Result
; ----------------------------------------------------------------------
_mia_call_long:
    sta MIA_OP
    jsr MIA_SPIN
    ldy MIA_SREG
    sty sreg
    ldy MIA_SREG+1
    sty sreg+1
    rts

; int __fastcall__ mia_call_int_errno(unsigned char op);
; ----------------------------------------------------------------------
; Function to call a MIA function that returns an int and sets errno
; ----------------------------------------------------------------------
; Input:        --> Operation code
; Output:       --> Result
; ----------------------------------------------------------------------
_mia_call_int_errno:
    sta MIA_OP
    jsr MIA_SPIN
    ldx MIA_X
    bmi ERROR
    rts

; long __fastcall__ mia_call_long_errno(unsigned char op);
; ----------------------------------------------------------------------
; Function to call a MIA function that returns a long and sets errno
; ----------------------------------------------------------------------
; Input:        --> Operation code
; Output:       --> Result
; ----------------------------------------------------------------------
_mia_call_long_errno:
    jsr _mia_call_long
    bmi ERROR
    rts

; void __fastcall__ mia_call_void(unsigned char op);
; ----------------------------------------------------------------------
; Function to call a MIA function that returns void
; ----------------------------------------------------------------------
; Input:        --> Operation code
; ----------------------------------------------------------------------
_mia_call_void:
    sta MIA_OP
    rts

ERROR:
    lda MIA_ERRNO
.ifdef OLD_CC65
    jmp __mappederrno
.else
    jmp ___mappederrno
.endif

