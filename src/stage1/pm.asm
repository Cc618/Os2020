; Set up protected mode (32 bits)

; 32 bits protected mode entry
protectedMain:
	; Set the data segment
	mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	; Update stack base to the top of the free space
    mov ebp, KERNEL_STACK_TOP
    mov esp, ebp

    ; Move variables in stage2 memory
    mov ax, [loadedSectors]
	mov [STAGE2_OFFSET + SHIFT_KERNEL_SECTOR_BEGIN], ax

    ; Give control to the second stage boot loader
    call STAGE2_OFFSET
