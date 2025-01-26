; LOCI API CC65 library
;
; Based on:
; - LOCI ROM by Sodiumlightbaby, 2024, https://github.com/sodiumlb/loci-rom
; - Picocomputer 6502 by Rumbledethumps, 2023, https://github.com/picocomputer/rp6502
;
; Adapted and extended by Xander Mol, 2025

.constructor initmainargs, 24
.import __argc, __argv

.segment "ONCE"

; Internal function to initialize main arguments
.proc initmainargs

    rts

.endproc
