
; Offset of the top of the boot stack (as segment = divided by 16)
%define BOOT_STACK_HIGH 0x8000

; To stop loading we look at this magic dword
%define END_OF_STAGE2_LOW 0x4F42
%define END_OF_STAGE2_HIGH 0x544F
