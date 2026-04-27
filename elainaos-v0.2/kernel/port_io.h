#ifndef PORT_IO_H
#define PORT_IO_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    // Ngăn compiler xáo trộn thứ tự lệnh với memory
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    // Tương tự outb
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0); // Delay ~1us qua POST code port
}

#endif
