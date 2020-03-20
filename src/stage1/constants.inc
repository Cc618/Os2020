; Where the second stage is loaded
%define STAGE2_OFFSET 0x8000

; Offset of the top of the stage 1 bootloader stack (as segment = divided by 16)
%define STAGE1_STACK_TOP 0x8000

; Top of the stack of the kernel
%define KERNEL_STACK_TOP 0xF000000

; To stop loading we look at this magic dword (BOOT)
%define END_OF_STAGE2_LOW 0x4F42
%define END_OF_STAGE2_HIGH 0x544F
