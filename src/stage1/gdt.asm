; The GDT

gdt_start:
	; Null 8 bytes
	dq 0x0

; Code segment, executable unlike the data segment.
; Base = 0x00000000, Limit = 0xFFFFF
gdt_kernel_code: 
	; Limit (0:15)
	dw 0xFFFF
	; Base (0:15)
	dw 0x0
	; Base (16:23)
	db 0x0
	; Flags 
	db 10011010b	; = 0x9A
	; Flags (4 bits) + limit (16:29)
	db 11001111b
	; Base
	db 0x0

; Data segment, identical to the code segment without the executable flag
gdt_kernel_data:
	; Limit (0:15)
	dw 0xFFFF
	; Base (0:15)
	dw 0x0
	; Base (16:23)
	db 0x0
	; Flags 
	db 10010010b	; = 0x92
	; Flags (4 bits) + limit (16:29)
	db 11001111b
	; Base
	db 0x0

; To compute the size of the GDT
gdt_end:

; GDT descriptor
gdt_descriptor:
	; Size - 1
	dw gdt_end - gdt_start - 1
	; The pointer to the GDT
	dd gdt_start

; Constants for setting the current used segment (with setting ds to these constants)
CODE_SEG: equ gdt_kernel_code - gdt_start
DATA_SEG: equ gdt_kernel_data - gdt_start
