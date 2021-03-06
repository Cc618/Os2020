; To call syscalls from user level with custom args (not int)

; To have syscall ids
%include "k/syscalls.inc"

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

; Sys
DEF_SYSC_1 fatal,       SYS_FATAL
DEF_SYSC_4 enter,       SYS_ENTER
DEF_SYSC_3 terminate,   SYS_TERMINATE

; Io
DEF_SYSC_2 open,    SYS_OPEN
DEF_SYSC_3 read,    SYS_READ
DEF_SYSC_3 write,   SYS_WRITE
DEF_SYSC_1 close,   SYS_CLOSE
DEF_SYSC_1 pipe,    SYS_PIPE

; Files
DEF_SYSC_2 ls,      SYS_LS
DEF_SYSC_2 context, SYS_CONTEXT
DEF_SYSC_2 touch,   SYS_TOUCH
DEF_SYSC_2 finfo,   SYS_FINFO
