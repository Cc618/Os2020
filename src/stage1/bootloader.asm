; This file gathers all source files to make the boot loader

[org 0x7C00]
[bits 16]

%include "constants.inc"

bootloader_begin:

; Os entry
%include "boot.asm"

; Functions used by boot.asm
%include "utils.asm"

; GDT in one file
%include "gdt.asm"

[bits 32]

; Prepare second stage bootloader
%include "pm.asm"

; Pad and add the magic number
times 0x200 - 2 - ($ - bootloader_begin) db 0

; Boot magic number
dw 0xAA55
