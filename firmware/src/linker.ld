/* ESP32 Linker Script */
/* Defines memory layout and sections */

MEMORY
{
    /* RAM: 4MB starting at 0x3FF80000 */
    ram (rwx) : ORIGIN = 0x40080000, LENGTH = 4M
}

/* Entry point */
ENTRY(_start)

SECTIONS
{
    /* Code section */
    .text : {
        *(.text.startup)
        *(.text)
        *(.text.*)
    } > ram
    
    /* Read-only data */
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > ram
    
    /* Read-write data */
    .data : {
        __data_start = .;
        *(.data)
        *(.data.*)
        __data_end = .;
    } > ram
    
    /* Uninitialized data */
    .bss : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    } > ram
    
    /* Stack */
    .stack : {
        . = ALIGN(16);
        __stack_start = .;
        . += 4096;  /* 4KB stack */
        __stack_end = .;
    } > ram
    
    /* Discard unused sections */
    /DISCARD/ : {
        *(.comment)
        *(.gnu*)
        *(.note*)
        *(.eh_frame*)
    }
} 