; This file gathers all source files to make the boot loader

[bits 16]
[org 0x7C00]

%include "constants.inc"

bootloader_begin:

; Os entry
%include "boot.asm"

; Loads the second stage bootloader
%include "loader.asm"

; Functions used by boot and loader
%include "utils.asm"

; Pad and add the magic number
times 0x200 - 2 - ($ - bootloader_begin) db 0

; Boot magic number
dw 0xAA55
