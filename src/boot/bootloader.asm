; This file gathers all source files to make the boot loader

[bits 16]
[org 0x7C00]

%include "constants.asm"

bootloader_begin:

%include "boot.asm"

; Pad and add the magic number
times 0x200 - 2 - (bootloader_begin - $$)
dw 0x55AA

; We are at offset 0x7E00
%include "loader.asm"

; Terminate sector
align 0x200
