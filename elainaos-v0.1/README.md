# ElainaOS v0.1 

Một hệ điều hành tối giản được phát triển bởi Yuri08 từ năm lớp 9. Bao gồm:

- Bootloader 512 byte (ASM)
- Kernel đơn giản in text (C)
- Ngôn ngữ Elaina đời đầu (giống Forth hoặc bytecode stack)
- Không có MMU, chưa có syscall phức tạp
- Tự build bằng `nasm`, `gcc`, `dd`

### Build:
```sh
nasm -f bin boot/boot.asm -o boot.bin
gcc -m32 -ffreestanding -c kernel/kernel.c -o kernel.o
ld -Ttext 0x1000 -o kernel.bin kernel.o
cat boot.bin kernel.bin > elainaos.img
