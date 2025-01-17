; LOCI API CC65 library
; Overlay RAM functions
;
; Based on:
; - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
; - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
;
; Apapted and extended by Xander Mol, 2025
        
        
        .export         _enable_overlay_ram, _disable_overlay_ram

        MICRODISCCFG    := $0314

.code

; void __fastcall__ _enable_overlay_ram (void);
; ----------------------------------------------------------------------
; Function to enable overlay RAM
; ----------------------------------------------------------------------
; NB:
; This function disables interrupts and disables Oric ROM
; Only ROM safe functions can be called until overlay RAM is disabled again
; ----------------------------------------------------------------------
_enable_overlay_ram:
        sei                             ; Disable interrupts
        lda      #%11111101             ; Enable overlay RAM
        sta      MICRODISCCFG           ; Write to register
        rts

; void __fastcall__ disable_overlay_ram (void);
; ----------------------------------------------------------------------
; Function to disable overlay RAM
; ----------------------------------------------------------------------
; NB:
; Only call after prior call to _enable_overlay_ram
; ----------------------------------------------------------------------
_disable_overlay_ram:
        lda      #%11111111             ; Disable overlay RAM
        sta      MICRODISCCFG           ; Write to register
        cli                             ; Enable interrupts
        rts