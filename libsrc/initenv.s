; LOCI API CC65 library
;
; Based on:
; - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
; - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
;
; Apapted and extended by Xander Mol, 2025

.constructor initenv, 24
.import __environ, __envcount, __envsize

.segment "ONCE"

; Internal routine to copy environment variables to XRAM
.proc initenv

    rts

.endproc
