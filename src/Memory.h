#pragma once

#include <vector>
#include <cstdint>
#include <map>


class Memory {
private:
    static constexpr size_t RAM_SIZE = 4 * 1024 * 1024; // 4MB
    static constexpr uint32_t RAM_BASE = 0x3FF80000;
    static constexpr uint32_t RAM_END = 0x3FFFFFFF;
    
    std::vector<uint8_t> ram;
    std::map<uint32_t, uint32_t> peripheralMappings; // address -> peripheral_id

public:
    Memory();
    ~Memory() = default;

    // Core memory access
    uint8_t read8(uint32_t address) const;
    uint16_t read16(uint32_t address) const;
    uint32_t read32(uint32_t address) const;
    
    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    void write32(uint32_t address, uint32_t value);
    
    // Bulk operations
    void writeBytes(uint32_t address, const std::vector<uint8_t>& data);
    std::vector<uint8_t> readBytes(uint32_t address, size_t length) const;
    
    // Memory validation
    bool isValidAddress(uint32_t address) const;
    bool isRAMAddress(uint32_t address) const;
    bool isPeripheralAddress(uint32_t address) const;
    
    // Peripheral mapping
    void mapPeripheral(uint32_t address, uint32_t peripheralId);
    void unmapPeripheral(uint32_t address);
    uint32_t getPeripheralId(uint32_t address) const;
    
    // Debug
    void dumpMemory(uint32_t address, size_t length) const;
    size_t getRAMSize() const { return RAM_SIZE; }
    uint32_t getRAMBase() const { return RAM_BASE; }
    uint32_t getRAMEnd() const { return RAM_END; }
}; 