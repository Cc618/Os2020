; Special sections

extern main

; This section is always before other sections in the second stage bootloader
section .entry
    jmp main

; This section is after other sections
section .end
    ; TODO : Avoid fill with 0
    times 508 db 0

    ; End Of Stage 2 magic number
    db 'BOOT'

