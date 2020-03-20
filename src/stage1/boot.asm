
; Os entry
entry:
	; Default drive id
	mov byte [defaultDrive], dl

	; Setup stack
	mov bp, BOOT_STACK_HIGH
	mov sp, bp


	; Loader
	; TODO : mv in loader.asm

	; push word 1
	; push word 0x7E00
	; call loadSector

	; mov ax, [0x7E00]
	; cmp ax, 0
	; je end

    mov bx, OK
    call print

	; TODO : Change
end:
	; TODO : Error function
	; mov bx, ERR_LOAD
	; call error

    jmp $


; Loads a sector from the disk
; - readSector, byte (arg as word) : Which sector to read from disk
; - writeOffset, ptr16 : Where to write the sector
loadSector:
    push bp
	mov	bp, sp

	; Clear carry flag (set to no error)
	clc

	; cl is the sector to read
	mov cx, 0x1 ; [bp + 4]
	
	; bx is the write offset
	mov bx, 0x7E00 ; [bp + 6]

	; 1 sector to read
	mov al, 1

	; Cylinder 0
	mov ch, 0

	; Head 0
	mov dh, 0

	; Drive, same drive as when we have booted
	mov dl, [defaultDrive]

	; Call read function
	mov ah, 2
	int 13h












	; ; - Load kernel - ;
	; ; Set kernel location (es:bx)
	; mov bx, (KERNEL_OFFSET >> 4)
	; mov es, bx
	; xor bx, bx

	; ; Set the sectors to read number
	; mov al, byte [LOAD_SECTORS_OFFSET]

	; ; Cylinder (0)
	; xor ch, ch

	; ; Sector 2 (the boot is the sector 1)
	; mov cl, 2

	; ; Head (0)
	; xor dh, dh

	; ; Drive, same drive as when we have booted
	; mov dl, [defaultDrive]

	; ; Call read function
	; mov ah, 2
	; int 13h

	; ; - Check errors - ;
	; ; Carry flag error = can't read
	; jnc .noErrorRead

	; ; Cannot load kernel (read)
	; mov si, STR_ERROR_LOAD_READ
	; call print
	; jmp end

	; .noErrorRead:
	; ; al is the number of sectors read
	; cmp al, byte [LOAD_SECTORS_OFFSET]
	; je .noErrorSector

	; ; Cannot load kernel (load)
	; mov si, STR_ERROR_LOAD_SECTORS
	; call print
	; jmp end

	; .noErrorSector:
	; popa


	leave
	ret






; --- Variables --- ;
defaultDrive: db 0

; --- Constants --- ;
OK: db 'OK !', 0
ERR_LOAD: db "Can't read disk", 0
