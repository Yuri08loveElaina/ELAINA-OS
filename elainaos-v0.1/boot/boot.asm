[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov si, boot_msg
    call print_string

    jmp 0x0000:0x1000        ; Nhảy đến kernel tại 0x1000

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

boot_msg db "Booting ElainaOS v0.1...", 0

times 510-($-$$) db 0
dw 0xAA55
