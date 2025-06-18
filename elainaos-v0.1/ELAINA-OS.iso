#!/bin/bash
# build_elainaos_iso.sh - Script build ISO cho ElainaOS v0.1
# Signature: github.com/Yuri08loveElaina - Elaina Core Division

set -e

OUT_ISO=elainaos.iso
ISO_DIR=isofiles
BOOT_DIR=$ISO_DIR/boot
KERNEL=kernel.bin
BOOTLOADER=bootloader.bin

# Kiểm tra file cần thiết
if [[ ! -f $KERNEL || ! -f $BOOTLOADER ]]; then
  echo "[!] Thiếu $KERNEL hoặc $BOOTLOADER. Hãy biên dịch trước."
  exit 1
fi

# Tạo thư mục ISO
rm -rf $ISO_DIR
mkdir -p $BOOT_DIR/grub
cp $KERNEL $BOOT_DIR/kernel.bin
cp $BOOTLOADER $BOOT_DIR/bootloader.bin

# Tạo file cấu hình GRUB
cat > $BOOT_DIR/grub/grub.cfg <<EOF
set timeout=0
set default=0

menuentry "ElainaOS v0.1" {
    multiboot /boot/kernel.bin
    boot
}
EOF

# Tạo file ISO
grub-mkrescue -o $OUT_ISO $ISO_DIR

# Chữ ký GPG
echo "Signed by Elaina Core (Yuri08) - $(date)" | gpg --clearsign > elaina_signature.txt

# Thông báo hoàn tất
echo "[+] Đã tạo file ISO: $OUT_ISO"
echo "[i] Chạy với: qemu-system-i386 -cdrom $OUT_ISO -m 128M -nographic"
