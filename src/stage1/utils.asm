; Functions used to boot
; These functions use the calling convention described in file doc/conventions.md

; --- Printing --- ;
; When there is an error, prints message and halt here
; - str, const char* : String to print
error:
    push bx

    mov bx, error.ERROR_MSG
    call print

    ; Restore arg
    pop bx
    call print

    ; Halt the system
    jmp $

error.ERROR_MSG: db 'Fatal Error :', 0

; Prints with the BIOS the null terminated string
; - str, const char* : String to print
; * Adds also a line feed
print:
    ; si is the string
    mov si, bx

	; - Loop - ;
	print.loop:
		; Retrieve the char pointed by si
		mov al, [si]

		; Break if it's a null char
		cmp al, 0
		je print.loop_end

		; Print the char
        mov bl, al
		call putchar

		inc si

		jmp print.loop

	print.loop_end:

    ; CRLF
    mov bl, 0xD
    call putchar
    mov bl, 0xA
    call putchar

	ret


; Prints a char
; chr, char : Char to print
putchar:
    ; putchar function of BIOS
	mov ah, 0x0E

    ; Arg 1 is at bp + 4 offset since we are in 16 bits
    mov al, bl
	int 10h
    
    ret


; --- Files --- ;
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

