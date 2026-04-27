OUTPUT_FORMAT("elf32-i386")
ENTRY(kernel_main)

SECTIONS {
    . = 0x10000;     // base address khớp với boot jmp
    
    .text : {
        *(.text)
        *(.rodata)   
    }
    
    .data : {
        *(.data)
    }
    
    .bss (NOLOAD) : {
        *(.bss)
        *(COMMON)    
    }
    
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note*)
    }
    
    end_of_kernel = .; // đánh dấu ranh giới cuối để khỏi ghi đè nhầm
}
