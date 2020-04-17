; To call syscalls from user level with custom args (not int)

; Syscall args :
; eax = syscall id / return
; ebx = 1st arg
; ecx = 2nd arg
; edx = 3rd arg
; edi = 4th arg

; Defines a syscall with 1 arg
; Args : name, syscall id
%macro DEF_SYSC_1 2
global %1
%1:
    push ebp
	mov ebp, esp

    push ebx

    ; Move args from stack to registers
    mov eax, %2
    mov ebx, [ebp + 8]

    ; Syscall
    int 0x80

    pop ebx

    leave
    ret
%endmacro

%macro DEF_SYSC_2 2
global %1
%1:
    push ebp
	mov ebp, esp

    push ebx

    ; Move args from stack to registers
    mov eax, %2
    mov ebx, [ebp + 8]
    mov ecx, [ebp + 12]

    ; Syscall
    int 0x80

    pop ebx

    leave
    ret
%endmacro

%macro DEF_SYSC_3 2
global %1
%1:
    push ebp
	mov ebp, esp

    push ebx
    push edi

    ; Move args from stack to registers
    mov eax, %2
    mov ebx, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]
    mov edi, [ebp + 20]

    ; Syscall
    int 0x80

    pop edi
    pop ebx

    leave
    ret
%endmacro

%macro DEF_SYSC_4 2
global %1
%1:
    push ebp
	mov ebp, esp

    push ebx

    ; Move args from stack to registers
    mov eax, %2
    mov ebx, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]
    mov edx, [ebp + 16]

    ; Syscall
    int 0x80

    pop ebx

    leave
    ret
%endmacro





; TODO : Use defines for ids
DEF_SYSC_1 fatal,   0x02
DEF_SYSC_2 putc,    0x10
DEF_SYSC_2 strcon,  0x20


