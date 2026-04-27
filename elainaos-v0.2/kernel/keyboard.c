#include "keyboard.h"
#include "vga.h"
#include "port_io.h"
#include "pic.h"
#include "idt.h"

// map scancode set 1
static unsigned char kbdus[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', 0, // 0x0E: backspace -> map bằng 0 để khỏi in rác
    0, 'q','w','e','r','t','y','u','i','o','p','[',']','\n',   // 0x0F: tab -> map bằng 0
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' '
};

static void keyboard_handler(struct registers *r) {
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        // key release, bỏ qua
    } else {
        // xử lý riêng cho backspace vì vga_putchar không hỗ trợ '\b'
        if (scancode == 0x0E) {
            vga_backspace(VGA_WHITE);
        } else if (kbdus[scancode]) {
            vga_putchar(kbdus[scancode], VGA_WHITE);
        }
    }
    pic_send_eoi(1);
}

void keyboard_install() {
    // irq1 = interrupt 33 sau khi remap pic
    register_isr_handler(33, keyboard_handler);
}
