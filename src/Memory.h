#pragma once

#include <vector>
#include <cstdint>
#include <map>
#include <functional>


class Memory {
private:
    static constexpr size_t RAM_SIZE = 4 * 1024 * 1024; // 4MB
    static constexpr uint32_t RAM_BASE = 0x3FF80000;
    static constexpr uint32_t RAM_END = 0x3FFFFFFF;
    
    std::vector<uint8_t> ram;
    
    std::function<uint8_t(uint32_t)> peripheralRead8Callback;
    std::function<uint16_t(uint32_t)> peripheralRead16Callback;
    std::function<uint32_t(uint32_t)> peripheralRead32Callback;
    std::function<void(uint32_t, uint8_t)> peripheralWrite8Callback;
    std::function<void(uint32_t, uint16_t)> peripheralWrite16Callback;
    std::function<void(uint32_t, uint32_t)> peripheralWrite32Callback;

public:
    Memory();
    ~Memory() = default;

    uint8_t read8(uint32_t address) const;
    uint16_t read16(uint32_t address) const;
    uint32_t read32(uint32_t address) const;
    
    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    void write32(uint32_t address, uint32_t value);
    
    void writeBytes(uint32_t address, const std::vector<uint8_t>& data);
    std::vector<uint8_t> readBytes(uint32_t address, size_t length) const;
    
    bool isValidAddress(uint32_t address) const;
    bool isRAMAddress(uint32_t address) const;
    bool isPeripheralAddress(uint32_t address) const;
    
    void setPeripheralCallbacks(
        std::function<uint8_t(uint32_t)> read8,
        std::function<uint16_t(uint32_t)> read16,
        std::function<uint32_t(uint32_t)> read32,
        std::function<void(uint32_t, uint8_t)> write8,
        std::function<void(uint32_t, uint16_t)> write16,
        std::function<void(uint32_t, uint32_t)> write32
    );
    
    void dumpMemory(uint32_t address, size_t length) const;
    size_t getRAMSize() const { return RAM_SIZE; }
    uint32_t getRAMBase() const { return RAM_BASE; }
    uint32_t getRAMEnd() const { return RAM_END; }
}; 