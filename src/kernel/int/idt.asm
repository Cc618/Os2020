; The interrupt descriptor table

extern onSyscall

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15
global irq128

extern irq0Handler
extern irq1Handler
extern irq2Handler
extern irq3Handler
extern irq4Handler
extern irq5Handler
extern irq6Handler
extern irq7Handler
extern irq8Handler
extern irq9Handler
extern irq10Handler
extern irq11Handler
extern irq12Handler
extern irq13Handler
extern irq14Handler
extern irq15Handler

; Inits the IDT in the idtDescriptor variable
extern idtDescriptor
global loadIDT
loadIDT:
	lidt [idtDescriptor]
	sti

	ret

; Timer
irq0:
	pusha
	call irq0Handler
	popa
	iret

; Keyboard
irq1:
	pusha
	call irq1Handler
	popa
	iret

irq2:
	pusha
	call irq2Handler
	popa
	iret

irq3:
	pusha
	call irq3Handler
	popa
	iret

irq4:
	pusha
	call irq4Handler
	popa
	iret

irq5:
	pusha
	call irq5Handler
	popa
	iret

irq6:
	pusha
	call irq6Handler
	popa
	iret

irq7:
	pusha
	call irq7Handler
	popa
	iret

irq8:
	pusha
	call irq8Handler
	popa
	iret

irq9:
	pusha
	call irq9Handler
	popa
	iret

irq10:
	pusha
	call irq10Handler
	popa
	iret

irq11:
	pusha
	call irq11Handler
	popa
	iret

irq12:
	pusha
	call irq12Handler
	popa
	iret

irq13:
	pusha
	call irq13Handler
	popa
	iret

irq14:
	pusha
	call irq14Handler
	popa
	iret

irq15:
	pusha
	call irq15Handler
	popa
	iret

; Syscall
irq128:
	push ebp
	mov ebp, esp

	; To return eax
	push 0
	pusha

	call onSyscall

	; Save return
	mov dword [ebp - 4], eax

	popa
	pop eax

    leave

	iret
