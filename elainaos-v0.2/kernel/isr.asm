[bits 32]
extern idt_handle_exception

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push dword 0         ; đệm cho đều
    push dword %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    push dword %1
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0    ; chia cho 0
ISR_ERRCODE 13     ; GPF quen thuộc

isr_common_stub:
    ; đẩy tay cho khớp struct bên C
    push eax
    push ecx
    push edx
    push ebx
    push esp
    push ebp
    push ds
    push es
    push fs
    push gs
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp         ; quăng stack cho hàm C
    call idt_handle_exception
    add esp, 4
    
    pop gs
    pop fs
    pop es
    pop ds
    pop ebp
    add esp, 4       ; vứt cái esp rác
    pop ebx
    pop edx
    pop ecx
    pop eax
    
    add esp, 8       ; dọn 2 cái số ở đầu
    iret
