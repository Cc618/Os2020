; Special sections

extern main

; This section is always before other sections in the second stage bootloader
section .entry
    ; --- Entry --- ;
jmp entry

; Ensure that we are 16 bytes after the begining of the file
align 16

    ; --- Variables --- ;
global KERNEL_SECTOR_BEGIN
KERNEL_SECTOR_BEGIN: dw 0

entry:
    call main

; This section is after other sections
section .end
    ; --- End Of Stage2 --- ;
    ; TODO : Avoid fill with 0
    times 508 db 0

    ; End Of Stage 2 magic number
    db 'BOOT'

