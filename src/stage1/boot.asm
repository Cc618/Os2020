
; Os entry
entry:
	; Default drive id
	mov byte [defaultDrive], dl

	; Setup stack
	mov bp, BOOT_STACK_HIGH
	mov sp, bp

	; Loader
	; TODO : mv in loader.asm ???
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
	mov ax, 0x0800
	mov es, ax
	mov ax, [es:bx]
	add bx, 2
	mov dx, [es:bx]

	pop es

	; Check end of stage 2 magic dword
	cmp ax, END_OF_STAGE2_LOW
	jne stage2NotLoaded
	cmp dx, END_OF_STAGE2_HIGH
	je stage2Loaded

stage2NotLoaded:
	; The start offset must be incremented of 512 = 508 + 2 + 2
	add bx, 2
	inc cx
	
	jmp loadStage2.loadCurrentSector

stage2Loaded:
	; TODO
    mov bx, OK
    call print

	; TODO
	jmp $


	; TODO : Change
end:
	; TODO : Error function
	mov bx, ERR_LOAD
	call error

    jmp $




; Loads a sector from the disk
; - writeShift, short : Where to write the sector, from 0x8000 (Write offset = 0x8000 + writeShift)
; - readSector, byte : Which sector to read from disk (Boot is sector 1)
loadSector:
	pusha

	; Clear carry flag (set to no error)
	clc

	; Write address (es:bx)
	; The stage 2 is loaded at 0x8000
	mov ax, 0x0800
	mov es, ax

	; Which sector to read is cl

	; Read sectors command
	mov ah, 2

	; 1 sector to read
	mov al, 1

	; Cylinder = 0
	mov dh, 0

	; Head = 0
	mov ch, 0

	; Drive
	mov dl, [defaultDrive]

	int 13h

	; Check errors
	; Carry flag error = can't read
	jnc .noError

	; Cannot load kernel
	mov bx, ERR_LOAD
	call error

loadSector.noError:

	popa

	ret


; --- Variables --- ;
defaultDrive: db 0

; --- Constants --- ;
OK: db 'OK !', 0
OK2: db 'OK2 !', 0
ERR_LOAD: db "Can't read disk", 0
