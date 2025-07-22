#include "UART.h"
#include <iostream>
#include <iomanip>

UART::UART() : Peripheral(0x3FF40000, 0x100), 
                dataRegister(0), statusRegister(0), 
                controlRegister(0), baudRateRegister(115200),
                txReady(true), rxReady(false) {
    std::cout << "UART peripheral initialized at 0x" << std::hex << baseAddress << std::dec << std::endl;
}

uint8_t UART::read8(uint32_t offset) const {
    switch (offset) {
        case UART_DATA_OFFSET:
            return dataRegister & 0xFF;
        case UART_STATUS_OFFSET:
            return statusRegister & 0xFF;
        case UART_CONTROL_OFFSET:
            return controlRegister & 0xFF;
        case UART_BAUD_OFFSET:
            return baudRateRegister & 0xFF;
        default:
            return 0;
    }
}

uint16_t UART::read16(uint32_t offset) const {
    if (offset % 2 != 0) return 0;
    
    uint16_t value = 0;
    value |= static_cast<uint16_t>(read8(offset));
    value |= static_cast<uint16_t>(read8(offset + 1)) << 8;
    return value;
}

uint32_t UART::read32(uint32_t offset) const {
    if (offset % 4 != 0) return 0;
    
    uint32_t value = 0;
    value |= static_cast<uint32_t>(read8(offset));
    value |= static_cast<uint32_t>(read8(offset + 1)) << 8;
    value |= static_cast<uint32_t>(read8(offset + 2)) << 16;
    value |= static_cast<uint32_t>(read8(offset + 3)) << 24;
    return value;
}

void UART::write8(uint32_t offset, uint8_t value) {
    switch (offset) {
        case UART_DATA_OFFSET:
            dataRegister = value;
            sendByte(value);
            break;
        case UART_STATUS_OFFSET:
            statusRegister = value;
            break;
        case UART_CONTROL_OFFSET:
            controlRegister = value;
            break;
        case UART_BAUD_OFFSET:
            baudRateRegister = (baudRateRegister & 0xFFFFFF00) | value;
            break;
    }
}

void UART::write16(uint32_t offset, uint16_t value) {
    if (offset % 2 != 0) return;
    
    write8(offset, value & 0xFF);
    write8(offset + 1, (value >> 8) & 0xFF);
}

void UART::write32(uint32_t offset, uint32_t value) {
    if (offset % 4 != 0) return;
    
    write8(offset, value & 0xFF);
    write8(offset + 1, (value >> 8) & 0xFF);
    write8(offset + 2, (value >> 16) & 0xFF);
    write8(offset + 3, (value >> 24) & 0xFF);
}

void UART::reset() {
    dataRegister = 0;
    statusRegister = 0;
    controlRegister = 0;
    baudRateRegister = 115200;
    txReady = true;
    rxReady = false;
    
    while (!txBuffer.empty()) txBuffer.pop();
    while (!rxBuffer.empty()) rxBuffer.pop();
}

void UART::update() {
    if (txReady) {
        statusRegister |= 0x01;  
    } else {
        statusRegister &= ~0x01;
    }
    
    if (rxReady) {
        statusRegister |= 0x02; 
    } else {
        statusRegister &= ~0x02;
    }
}

void UART::dumpRegisters() const {
    std::cout << "UART Registers:" << std::endl;
    std::cout << "  Data:      0x" << std::hex << std::setw(8) << std::setfill('0') << dataRegister << std::dec << std::endl;
    std::cout << "  Status:    0x" << std::hex << std::setw(8) << std::setfill('0') << statusRegister << std::dec << std::endl;
    std::cout << "  Control:   0x" << std::hex << std::setw(8) << std::setfill('0') << controlRegister << std::dec << std::endl;
    std::cout << "  Baud Rate: 0x" << std::hex << std::setw(8) << std::setfill('0') << baudRateRegister << std::dec << std::endl;
}

void UART::sendByte(uint8_t byte) {
    // For now, just print to console
    std::cout << static_cast<char>(byte);
    std::cout.flush();
}

uint8_t UART::receiveByte() {
    if (rxBuffer.empty()) {
        return 0;
    }
    
    uint8_t byte = rxBuffer.front();
    rxBuffer.pop();
    return byte;
}

bool UART::hasData() const {
    return !rxBuffer.empty();
}

void UART::print(const std::string& message) {
    for (char c : message) {
        sendByte(static_cast<uint8_t>(c));
    }
} 