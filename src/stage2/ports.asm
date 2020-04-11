; Outputs a byte from a port
global outb
outb:
	push ebp
	mov ebp, esp

	push ax
	push dx

	; dx = port, al = value
	mov dx, word [ebp + 12]
	mov al, byte [ebp + 8]
	out dx, al

	pop dx
	pop ax

	leave
	ret


; Outputs a word from a port
global outw
outw:
	push ebp
	mov ebp, esp

	push ax
	push dx

	; dx = port, ax = value
	mov dx, word [ebp + 12]
	mov ax, word [ebp + 8]
	out dx, ax

	pop dx
	pop ax

	leave
	ret


; Receives a byte from a port
global inb
inb:
	push ebp
	mov ebp, esp

	push dx

	; dx = port, al = value
	mov dx, word [ebp + 8]
	in al, dx

	pop dx

	leave
	ret


; Receives a word from a port
global inw
inw:
	push ebp
	mov ebp, esp

	push dx

	; dx = port, al = value
	mov dx, word [ebp + 8]
	in ax, dx

	pop dx

	leave
	ret
