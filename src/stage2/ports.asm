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










global loadAsm
loadAsm:
   mov     dx,1f6h         ;Drive and head port
   mov     al,0a0h         ;Drive 0, head 0
   out     dx,al

   mov     dx,1f2h         ;Sector count port
   mov     al,1            ;Read one sector
   out     dx,al

   mov     dx,1f3h         ;Sector number port
   mov     al, 1  ;;;          ;Read sector one
   out     dx,al

   mov     dx,1f4h         ;Cylinder low port
   mov     al,0            ;Cylinder 0
   out     dx,al

   mov     dx,1f5h         ;Cylinder high port
   mov     al,0            ;The rest of the cylinder 0
   out     dx,al

   mov     dx,1f7h         ;Command port
   mov     al,20h          ;Read with retry.
   out     dx,al

still_going:
   in      al,dx
   test    al,8            ;This means the sector buffer requires
            ;servicing.
   jz      still_going     ;Don't continue until the sector buffer
            ;is ready.

   mov     cx, 512 / 2        ;One sector /2
   mov     di, 0xB8000
   mov     dx, 1f0h         ;Data port - data comes in and out of here.
   rep     insw

