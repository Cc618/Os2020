; Special sections

extern main

; This section is always before other sections in the kernel
section .entry
    ; --- Entry --- ;
jmp entry

; Ensure that we are 16 bytes after the begining of the file
align 16

    ; --- Variables --- ;
; !!! Do NOT reorder these variables

; . = KERNEL + 16
; The first sector after the kernel
; This variable is set 
global FS_SECTOR
FS_SECTOR:
    dd 0

entry:
    call main

; This section is after other sections
section .end
    ; --- End Of Kernel --- ;
    ; TODO : Avoid fill with 0
    times 508 db 0

    ; End Of Kernel magic number
    db 'CORE'
