#include "idt.h"
#include "vga.h"

static struct idt_entry idt[256];
static struct idt_ptr idtp;

// callback table
static isr_t interrupt_handlers[256] = { 0 };

// từ file asm
extern void isr0();
extern void isr13();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

// trỏ vào từ asm stub
void idt_handle_exception(struct registers *r) {
    if (interrupt_handlers[r->int_no] != 0) {
        interrupt_handlers[r->int_no](r);
    } else {
        vga_print("EXCEPTION: ", VGA_RED);
        
        // dump mã lỗi ra màn hình
        char hex[] = "0x00";
        hex[2] = "0123456789ABCDEF"[(r->int_no >> 4) & 0xF];
        hex[3] = "0123456789ABCDEF"[r->int_no & 0xF];
        vga_print(hex, VGA_RED);
        vga_print(" Halting.\n", VGA_LIGHT_GREY);
        
        asm volatile("cli; hlt");
    }
}

void register_isr_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void idt_install() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    // clear hết
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // set vài cái cần thiết
    // 0x8E: interrupt gate 32-bit
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);

    asm volatile("lidt (%0)" : : "r"(&idtp));
}
