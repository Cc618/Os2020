; Loads the second stage bootloader, switch to protected mode

; Os entry
entry:
	; Default drive id
	mov byte [defaultDrive], dl

	; Setup stack
	mov bp, STAGE1_STACK_TOP
	mov sp, bp

	; Load stage 2 with BIOS
	call loadStage2

	; Go to Protected Mode
	call switchPm


; Procedure which loads the second stage bootloader
loadStage2:
	; bx is the start offset of the chunk we load
	mov bx, 0

	; cx is the sector we read
	mov cx, 2

loadStage2.loadCurrentSector:
	; Load the current sector
	push cx
	push bx
	call loadSector
	pop bx
	pop cx

	; Load possible end of stage2
	push es

	add bx, 508

	mov ax, STAGE2_OFFSET >> 4
	mov es, ax
	
	mov ax, [es:bx]
	add bx, 2
	mov dx, [es:bx]

	pop es

	; Check end of stage 2 magic dword
	cmp ax, END_OF_STAGE2_LOW
	jne loadStage2.notLoaded
	cmp dx, END_OF_STAGE2_HIGH
	je loadStage2.loaded

loadStage2.notLoaded:
	; The start offset must be incremented of 512 = 508 + 2 + 2
	add bx, 2
	inc cx

	jmp loadStage2.loadCurrentSector

loadStage2.loaded:

	ret


; Procedure to switch to the 32 bits protected mode
switchPm:
	; Disable interrupts
	cli

	; Load the GDT
    lgdt [gdt_descriptor]

	; Set the protected mode bit
    mov eax, cr0
    or eax, 1
    mov cr0, eax

	; Far jump with the code segment to 32 bits
    jmp CODE_SEG:protectedMain


; --- Variables --- ;
defaultDrive: db 0

; --- Constants --- ;
OK: db 'OK !', 0
ERR_LOAD: db "Can't read disk", 0
