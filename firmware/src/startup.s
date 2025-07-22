

    .section .text.startup
    .global _start
    .align 4

_start:
    movi    a1, 0x3FFE0000  
    
    movi    a2, __bss_start
    movi    a3, __bss_end
    sub     a3, a3, a2
    beqz    a3, 1f
    movi    a4, 0
    loop    a3, 2f
    s8i     a4, a2, 0
    addi    a2, a2, 1
2:  nop
1:
    
    /* Call main() */
    call0   main
    
    j       .

    .section .bss
    .align 4
    .space 4096 