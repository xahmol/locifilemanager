;)              _
;)  ___ ___ _ _|_|___ ___
;) |  _| .'|_'_| |_ -|_ -|
;) |_| |__,|_,_|_|___|___|
;)         raxiss (c) 2021
;)
;) GNU General Public License v3.0
;) See https://github.com/iss000/oricOpenLibrary/blob/main/LICENSE
;)

; ======================================================================
; IJK-driver code
; ======================================================================

;  VIA portA    IJK bits
;
;     PA0     -  RIGHT
;     PA1     -  LEFT
;     PA2     -  FIRE
;     PA3     -  DOWN
;     PA4     -  UP
;     PA5     -  DETECT
;     PA6     -  LSTICK
;     PA7     -  RSTICK

        .export _ijk_detect, _ijk_read, _ijk_present, _ijk_ljoy, _ijk_rjoy

; VIA ports and registers
        via_b         := $0300
        via_a         := $0301
        via_ddrb      := $0302
        via_ddra      := $0303
        via_aor       := $030f

; ======================================================================
.code

; Variables
; ----------------------------------------------------------------------
_ijk_present:    .byte   0               ; 0 - not present
_ijk_ljoy:      .byte   0               ; button status - left joystick
_ijk_rjoy:      .byte   0               ; button status - right joystick

; ----------------------------------------------------------------------
; Internal: Prepare IJK interface for reading
; ----------------------------------------------------------------------
ijk_prepare:
                ;ensure printer strobe is set to output
                lda     via_ddrb
                ora     #%00010000
                sta     via_ddrb

                ;set strobe low
                lda     via_b
                and     #%11101111
                sta     via_b     

                ;set top two bits of porta to output and rest as input
                lda     #%11000000
                sta     via_ddra
                rts

; ----------------------------------------------------------------------
; Internal: Release IJK interface
; ----------------------------------------------------------------------
ijk_release:
                ;set strobe high
                lda     via_b
                ora     #%00010000
                sta     via_b
                rts

; void ijk_detect(void);
; ----------------------------------------------------------------------
; Function to detect IJK interface
; ----------------------------------------------------------------------
; Output:   sets ijk_present to 1 if interface is connected, 0 otherwise
;           resets ijk_ljoy and ijk_rjoy to 0
; ----------------------------------------------------------------------
_ijk_detect:
                php
                sei
                ; preserve via port a
                lda     via_ddra
                pha
                lda     via_a
                pha

                lda     #$00
                sta     _ijk_present
                sta     _ijk_ljoy
                sta     _ijk_rjoy

                jsr     ijk_prepare

                ; set bits 7 and 6
                lda     #%11000000
                sta     via_aor

                ; read back and mask bit 5
                ; it will be 0 if interface is connected
                ; make it to 1 to avoid reverse logic
                lda     via_aor
                and     #%00100000
                eor     #%00100000
                sta     _ijk_present

                jsr     ijk_release

                ;restore via porta state
                pla
                sta     via_aor
                pla
                sta     via_ddra
                plp
                rts

; void ijk_read(void);
; ----------------------------------------------------------------------
; Function to read IJK interface
; ----------------------------------------------------------------------
; Output:   sets ijk_ljoy and ijk_rjoy to the joystick button status
; ----------------------------------------------------------------------
_ijk_read:
                php
                sei
                lda     via_ddra
                pha
                lda     via_a
                pha

                jsr     ijk_prepare

                ;select left joystick
                lda     #%01000000
                sta     via_aor
                ;read back left joystick state
                lda     via_aor
                ;mask out unused bits
                and     #%00011111
                ;invert bits
                eor     #%00011111
                ;store to variable
                sta     _ijk_ljoy

                ;select right joystick
                lda     #%10000000
                sta     via_aor
                ;read back right joystick state
                lda     via_aor
                ;mask out unused bits
                and     #%00011111
                ;invert bits
                eor     #%00011111
                ;store to variable
                sta     _ijk_rjoy

                jsr     ijk_release

                ;restore via porta state
                pla
                sta     via_aor
                pla
                sta     via_ddra
                plp
                rts