[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, boot_msg
    call print_string

    ; Load kernel (2nd sector -> 0x1000:0000)
    mov bx, 0x0000
    mov es, bx
    mov bx, 0x0000

    mov ah, 0x02
    mov al, 10          ; load 10 sectors
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, 0x00
    int 0x13

    jmp 0x1000:0000     ; jump to kernel

print_string:
    mov ah, 0x0E
.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

boot_msg db "ElainaOS v0.1 Booting...", 0

times 510 - ($ - $$) db 0
dw 0xAA55
