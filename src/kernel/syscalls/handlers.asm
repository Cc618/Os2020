extern malloc
extern free
extern stackStates
extern Vector_new
extern Vector_add
extern Vector_pop


global sys_enterProcess
sys_enterProcess:
    push ebp
	mov ebp, esp
    sub esp, 0x40

    ; Init stack states if necessary
    cmp dword [stackStates], 0
    jne .stackStatesInit

    call Vector_new
    mov [stackStates], eax

.stackStatesInit:
    ; Create the stack state
    ; sizeof(ebp) + sizeof(esp) = 8
    push 8
    call malloc

    ; eax is now the new state offset
    mov [eax], esp
    mov [eax + 4], ebp

    ; Push the new stack state
    push eax
    push dword [stackStates]
    call Vector_add

    ; Call entry
    mov ecx, [ebp + 8]
    push dword [ebp + 16]
    push dword [ebp + 12]
    call ecx

    ; Save return
    mov [ebp - 4], eax

    ; Pop and free state
    push dword [stackStates]
    call Vector_pop

    ; Free the state
    push eax
    call free

    ; Restore return
    mov eax, [ebp - 4]

    ; Normal exit, eax is the return
    jmp sys_enterProcess.onExit

; When we raise the termination signal, we jmp to this label
terminationReturn:
    ; Pop the state
    push dword [stackStates]
    call Vector_pop

    ; eax is the state, sync state
    mov esp, [eax]
    mov ebp, [eax + 4]

    ; Free the state
    push eax
    call free

    ; When termination, return value = -2
    mov eax, -2

sys_enterProcess.onExit:
    leave
    ret


global sys_terminate
sys_terminate:
    jmp terminationReturn
