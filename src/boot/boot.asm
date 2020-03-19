
entry:
    ; - Init - ;
	; Default drive id
	; mov byte [defaultDrive], dl

	; - Stack - ;
	; Setup stack
	mov bp, BOOT_STACK_HIGH
	mov sp, bp




    mov si, HW
    call print

    jmp $


; Prints with the BIOS the null terminated string pointed by si
; - si : Address of the string to print
; * Adds also a line feed
print:
	push ax

	; For the BIOS print function
	mov ah, 0x0E

	; - Loop - ;
	.loop:
		; Retrieve the char pointed by si in al
		mov al, [si]

		; Break if it's a null char
		cmp al, 0
		je .loop_end

		; Print the char
		int 10h

		inc si

		jmp .loop

	.loop_end:

	; - Line feed (CRLF) - ;
	mov al, 0xD
	int 10h
	mov al, 0xA
	int 10h

	pop ax

	ret


HW: db 'Hello World!', 0
