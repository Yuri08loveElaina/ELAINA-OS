[BITS 16]
[ORG 0x7C00]

start:
    cli                     ; Tắt ngắt trước khi config, tránh bị ngắt ngang khi đang đổi segment
    xor ax, ax
    mov ds, ax              ; Chưa biết DS đang trỏ đâu khi vừa boot, gán về 0 cho an toàn
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00          ; Đặt stack chạy ngược từ 0x7C00 xuống dưới

    mov si, boot_msg
    call print_16

    ; Reset ổ đĩa trước
    mov ah, 0x00
    int 0x13

    ; Đọc 20 sectors (10KB) của kernel lên RAM
    mov ax, 0x1000
    mov es, ax              ; Đích đến là 0x1000:0x0000 
    xor bx, bx

    mov ah, 0x02            ; Chức năng đọc sector của BIOS
    mov al, 20              ; Đọc 20 cái
    mov ch, 0x00            ; Cylinder 0
    mov cl, 0x02            ; Sector 2 (Sector 1 là cái file boot này, không đụng vào)
    mov dh, 0x00            ; Head 0
    mov dl, 0x80            ; Đọc từ ổ cứng đầu tiên (0x80). Nếu test trên Floppy thì đổi thành 0x00
    int 0x13
    jc disk_error           ; Cờ Carry = 1 là có lỗi đĩa

    ; Mở khóa A20 line để CPU truy cập được RAM vượt quá giới hạn 1MB
    in al, 0x92
    or al, 2                ; Đánh bit 1 lên
    out 0x92, al

    lgdt [gdt_descriptor]   ; Nạp bảng GDT vào thanh ghi của CPU

    ; Bật Protected Mode bằng cách set bit 0 (PE) trong thanh ghi CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump ép CPU xóa pipeline và chuyển sang dùng segment code mới (selector 0x08)
    jmp dword 0x08:0x10000

disk_error:
    mov si, err_msg
    call print_16
    jmp $                   ; Treo tại đây nếu lỗi

; Routine in chuỗi dùng BIOS interrupt 10h
print_16:
    mov ah, 0x0E
.next:
    lodsb                   ; Đọc 1 byte từ [SI] vào AL, tự tăng SI
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

boot_msg db "ElainaOS Bootloader loading kernel...", 13, 10, 0
err_msg db "FATAL: Disk read error!", 13, 10, 0

; Bảng GDT, bắt buộc align theo 8 byte
align 8
gdt_start:
    dq 0x0                  ; Segment null, quy định bắt buộc phải có cái này đầu tiên

    ; Code segment (Selector 0x08)
    dw 0xFFFF               ; Limit bits 0-15
    dw 0x0000               ; Base bits 0-15
    db 0x00                 ; Base bits 16-23
    db 10011010b            ; Access: P=1, DPL=0 (Ring 0), S=1 (Code/Data), Type=Code/Execute-Read
    db 11001111b            ; Flags: G=1 (4KB granularity), D=1 (32-bit), Limit bits 16-19 = 0xF
    db 0x00                 ; Base bits 24-31

    ; Data segment (Selector 0x10)
    dw 0xFFFF               
    dw 0x0000               
    db 0x00                 
    db 10010010b            ; Access: P=1, DPL=0, S=1, Type=Data/Read-Write
    db 11001111b            ; Flags giống code segment
    db 0x00                 
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Kích thước toàn bộ GDT
    dd gdt_start               ; Địa chỉ bắt đầu của GDT

; Đệm bằng 0 cho đến byte thứ 510, đánh dấu boot sector bằng 0xAA55
times 510 - ($ - $$) db 0
dw 0xAA55
