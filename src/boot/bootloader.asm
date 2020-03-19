; This file gathers all source files to make the boot loader

[bits 16]
[org 0x7C00]

%include "constants.inc"

bootloader_begin:

%include "boot.asm"

; Pad and add the magic number
times 0x200 - 2 - ($ - bootloader_begin) db 0

; Boot magic number
dw 0xAA55

; We are at offset 0x7E00
%include "loader.asm"

; Terminate sector
align 0x200
