
; Os entry
entry:
	; Default drive id
	mov byte [defaultDrive], dl

	; Setup stack
	mov bp, BOOT_STACK_HIGH
	mov sp, bp


	; Loader
	; TODO : mv in loader.asm

	mov bx, 0x0
	mov cx, 2
	call loadSector

	; mov ax, [0x7E00]
	; cmp ax, 0
	; je end

    ; mov bx, OK
	mov bx, 0x8000
    call print

	; TODO : Change
end:
	; TODO : Error function
	; mov bx, ERR_LOAD
	; call error

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
ERR_LOAD: db "Can't read disk", 0
