#include "vga.h"
#include "port_io.h"

// Thêm volatile chống GCC optimize bye luôn đoạn ghi memory
static volatile uint16_t* vga_buffer = VGA_MEMORY;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void vga_update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

static void vga_scroll(void) {
    if (cursor_y >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            // << ưu tiên hơn |, ngoãn lại cho chắc
            vga_buffer[i] = (uint16_t)((VGA_BLACK << 8) | ' ');
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_init(void) {
    for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        vga_buffer[i] = (uint16_t)((VGA_BLACK << 8) | ' ');
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_update_cursor();
}

void vga_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (uint16_t)((color << 8) | c);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    vga_scroll();
    vga_update_cursor();
}

void vga_backspace(uint8_t color) {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }
    vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (uint16_t)((color << 8) | ' ');
    vga_update_cursor();
}

void vga_print(const char* str, uint8_t color) {
    while (*str) {
        vga_putchar(*str++, color);
    }
}
