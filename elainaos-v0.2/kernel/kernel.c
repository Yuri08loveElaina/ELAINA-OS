#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "port_io.h"

void divide_by_zero_handler(struct registers *r) {
    vga_print("\nFATAL: Divide by Zero\n", VGA_RED);
    asm volatile("cli; hlt"); 
}

void gpf_handler(struct registers *r) {
    vga_print("\nFATAL: GPF\n", VGA_RED);
    asm volatile("cli; hlt");
}

void kernel_main() {
    vga_init();
    pic_remap(); 
    idt_install();
    register_isr_handler(0, divide_by_zero_handler);
    register_isr_handler(13, gpf_handler);
    
    keyboard_install();

    // 0xFD = 1111 1101 (mở bit 1)
    outb(PIC1_DATA, 0xFD);

    asm volatile("sti");

    vga_print("ElainaOS v0.1\n", VGA_LIGHT_GREEN);
    vga_print("> ", VGA_YELLOW);

    while(1) {
        asm volatile("hlt");
    }
}
