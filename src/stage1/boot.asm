
entry:
    ; - Init - ;
	; Default drive id
	; mov byte [defaultDrive], dl

	; - Stack - ;
	; Setup stack
	mov bp, BOOT_STACK_HIGH
	mov sp, bp



    push HW
    call print

    jmp $













HW: db 'Hello World!', 0
