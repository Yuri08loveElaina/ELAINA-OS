CC = gcc
CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wall -Wextra -Wno-unused-parameter
LD = ld
NASM = nasm

all: elainaos.img

boot.bin: boot/boot.asm
    $(NASM) -f bin $< -o $@


kernel.o: kernel/kernel.c
    $(CC) $(CFLAGS) -c $< -o $@
vga.o: kernel/vga.c
    $(CC) $(CFLAGS) -c $< -o $@
idt.o: kernel/idt.c
    $(CC) $(CFLAGS) -c $< -o $@
pic.o: kernel/pic.c
    $(CC) $(CFLAGS) -c $< -o $@
keyboard.o: kernel/keyboard.c
    $(CC) $(CFLAGS) -c $< -o $@

isr.o: kernel/isr.asm
    $(NASM) -f elf32 $< -o $@


kernel.bin: kernel.o vga.o idt.o pic.o keyboard.o isr.o
    $(LD) -m elf_i386 -T kernel/linker.ld --oformat binary -o kernel.tmp
    dd if=kernel.tmp of=$@ bs=1 skip=65536 2>/dev/null
    rm -f kernel.tmp

elainaos.img: boot.bin kernel.bin
    cat $^ > $@
    @echo "Build thanh cong: elainaos.img"

clean:
    rm -f *.bin *.o *.tmp
