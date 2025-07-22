#include <stdint.h>
#include <stddef.h>

#define UART_BASE 0x3FF40000
#define UART_DATA (UART_BASE + 0x00)

void uart_putc(char c);
void uart_puts(const char* str);
void delay(uint32_t cycles);

int main() {
    uart_puts("Hello from ESP32 firmware!\n");
    
    for (int i = 0; i < 5; i++) {
        uart_puts("Count: ");
        uart_putc('0' + i);
        uart_puts("\n");
        delay(1000000);
    }
    
    uart_puts("Firmware execution complete.\n");
    
    while (1) {
        delay(1000000);
    }
    
    return 0;
}

void uart_putc(char c) {
    volatile uint32_t* uart_data = (volatile uint32_t*)UART_DATA;
    *uart_data = (uint32_t)c;
}

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void delay(uint32_t cycles) {
    for (volatile uint32_t i = 0; i < cycles; i++) {
    }
} 