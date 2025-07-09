#pragma once

#include <cstdint>

class Peripheral {
protected:
    uint32_t baseAddress;
    uint32_t size;

public:
    Peripheral(uint32_t baseAddr, uint32_t peripheralSize);
    virtual ~Peripheral() = default;

    // Memory-mapped I/O interface
    virtual uint8_t read8(uint32_t offset) const = 0;
    virtual uint16_t read16(uint32_t offset) const = 0;
    virtual uint32_t read32(uint32_t offset) const = 0;
    
    virtual void write8(uint32_t offset, uint8_t value) = 0;
    virtual void write16(uint32_t offset, uint16_t value) = 0;
    virtual void write32(uint32_t offset, uint32_t value) = 0;
    
    virtual void reset() = 0;
    virtual void update() = 0;  
    
    bool isInRange(uint32_t address) const;
    uint32_t getBaseAddress() const { return baseAddress; }
    uint32_t getSize() const { return size; }
    
    virtual void dumpRegisters() const = 0;
}; 