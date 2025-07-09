#pragma once

#include "Peripheral.h"
#include <queue>
#include <string>


class UART : public Peripheral {
private:
    uint32_t dataRegister;      
    uint32_t statusRegister;    
    uint32_t controlRegister;  
    uint32_t baudRateRegister;  
    
    std::queue<uint8_t> txBuffer;
    std::queue<uint8_t> rxBuffer;
    bool txReady;
    bool rxReady;
    
    static constexpr uint32_t UART_DATA_OFFSET = 0x00;
    static constexpr uint32_t UART_STATUS_OFFSET = 0x04;
    static constexpr uint32_t UART_CONTROL_OFFSET = 0x08;
    static constexpr uint32_t UART_BAUD_OFFSET = 0x0C;

public:
    UART();
    ~UART() = default;

    uint8_t read8(uint32_t offset) const override;
    uint16_t read16(uint32_t offset) const override;
    uint32_t read32(uint32_t offset) const override;
    
    void write8(uint32_t offset, uint8_t value) override;
    void write16(uint32_t offset, uint16_t value) override;
    void write32(uint32_t offset, uint32_t value) override;
    
    void reset() override;
    void update() override;
    void dumpRegisters() const override;
    
    void sendByte(uint8_t byte);
    uint8_t receiveByte();
    bool hasData() const;
    void print(const std::string& message);
}; 