all: elainaos.iso

boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin

kernel.o: kernel.c
	i386-elf-gcc -ffreestanding -m32 -c kernel.c -o kernel.o

kernel.bin: kernel.o linker.ld
	i386-elf-ld -T linker.ld -o kernel.elf kernel.o
	i386-elf-objcopy -O binary kernel.elf kernel.bin

elainaos.img: boot.bin kernel.bin
	cat boot.bin kernel.bin > elainaos.img

elainaos.iso: elainaos.img
	mkdir -p isodir/boot
	cp elainaos.img isodir/boot/
	grub-mkrescue -o elainaos.iso isodir

clean:
	rm -rf *.bin *.o *.elf *.img *.iso isodir
