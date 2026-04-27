#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "port_io.h" 

void divide_by_zero_handler(struct registers *r) {
    vga_print("\nFATAL ERROR: Divide by Zero!\n", VGA_RED);
    asm volatile("cli; hlt"); 
}

void gpf_handler(struct registers *r) {
    vga_print("\nFATAL ERROR: General Protection Fault!\n", VGA_RED);
    asm volatile("cli; hlt");
}

void kernel_main() {
    vga_init();
    pic_remap();      //  đang mask hết cứng ngắc
    idt_install();
    
    register_isr_handler(0, divide_by_zero_handler);
    register_isr_handler(13, gpf_handler);
    
    keyboard_install();

    // bật lại IRQ1 . PIC mask hết mà không unmask lại thì gõ búa cũng không có tín hiệu
    outb(PIC1_DATA, inb(PIC1_DATA) & ~0x02);

    asm volatile("sti");

    vga_print("Welcome to ElainaOS v0.2 (32-bit Protected Mode)\n", VGA_LIGHT_GREEN);
    vga_print("VGA Driver, IDT, PIC remapped & Keyboard loaded.\n", VGA_LIGHT_GREY);
    vga_print("> ", VGA_YELLOW);

    while(1) {
        asm volatile("hlt");
    }
}
