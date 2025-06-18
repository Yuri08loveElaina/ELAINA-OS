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
cat boot.bin kernel.bin > elainaos.img```

```sh
make
qemu-system-i386 -cdrom elainaos.iso

Bạn sẽ thấy giao diện màn hình đen và dòng chữ:
ElainaOS v0.1 Booting...
Welcome to ElainaOS v0.1```

Hoặc Bạn chỉ cần:

- Đảm bảo có kernel.bin và bootloader.bin cùng thư mục.

- Cài sẵn grub-mkrescue (trong grub2 hoặc grub-pc-bin tùy distro).

- Chạy script:

chmod +x build_elainaos_iso.sh
./build_elainaos_iso.sh
File elainaos.iso sẽ chạy được trên QEMU hoặc ghi ra USB/CD nếu muốn test thật.
###✍️ Ghi chú Elaina Core
- Phiên bản này chưa có hệ thống syscall, FS hay usermode.
- Tác giả: Yuri08 - Elaina Core
- Ghi rõ nguồn nếu dùng lại: github.com/Yuri08loveElaina
###🔁lưu ý  Nếu lỗi do thiếu i386-elf-gcc:
Bạn có thể build thủ công:

wget https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
# Và build cross-compiler (mất khoảng 1h)
Hoặc dùng Docker OSDev image nếu không muốn cài trực tiếp.
