#ifndef IDT_H
#define IDT_H
#include <stdint.h>

// Entry IDT chuẩn 32-bit
struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;       
    uint8_t  zero;      
    uint8_t  flags;     
    uint16_t base_hi;   
} __attribute__((packed));

// Truyền cái này vào thanh ghi cho lệnh lidt
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Layout stack khi vào ISR, phải define đúng thứ tự với bên file asm
struct registers {
    uint32_t gs, fs, es, ds, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install(void);
typedef void (*isr_t)(struct registers *);
void register_isr_handler(uint8_t n, isr_t handler);

#endif
