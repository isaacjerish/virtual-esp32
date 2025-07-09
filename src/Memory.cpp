#include "Memory.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstring>

Memory::Memory() : ram(RAM_SIZE, 0) {
    std::cout << "Memory initialized: " << (RAM_SIZE / 1024 / 1024) << "MB RAM" << std::endl;
    std::cout << "RAM range: 0x" << std::hex << RAM_BASE << " - 0x" << RAM_END << std::dec << std::endl;
}

uint8_t Memory::read8(uint32_t address) const {
    if (!isValidAddress(address)) {
        throw std::out_of_range("Invalid memory address: 0x" + std::to_string(address));
    }
    
    if (isRAMAddress(address)) {
        uint32_t offset = address - RAM_BASE;
        return ram[offset];
    }
    
    // For peripheral addresses, return 0 for now
    // This will be handled by peripheral classes later
    return 0;
}

uint16_t Memory::read16(uint32_t address) const {
    if (address % 2 != 0) {
        throw std::runtime_error("Unaligned 16-bit read at address: 0x" + std::to_string(address));
    }
    
    uint16_t value = 0;
    value |= static_cast<uint16_t>(read8(address));
    value |= static_cast<uint16_t>(read8(address + 1)) << 8;
    return value;
}

uint32_t Memory::read32(uint32_t address) const {
    if (address % 4 != 0) {
        throw std::runtime_error("Unaligned 32-bit read at address: 0x" + std::to_string(address));
    }
    
    uint32_t value = 0;
    value |= static_cast<uint32_t>(read8(address));
    value |= static_cast<uint32_t>(read8(address + 1)) << 8;
    value |= static_cast<uint32_t>(read8(address + 2)) << 16;
    value |= static_cast<uint32_t>(read8(address + 3)) << 24;
    return value;
}

void Memory::write8(uint32_t address, uint8_t value) {
    if (!isValidAddress(address)) {
        throw std::out_of_range("Invalid memory address: 0x" + std::to_string(address));
    }
    
    if (isRAMAddress(address)) {
        uint32_t offset = address - RAM_BASE;
        ram[offset] = value;
    } else {
        // For peripheral addresses, this will be handled by peripheral classes
        // For now, just ignore writes to peripheral space
    }
}

void Memory::write16(uint32_t address, uint16_t value) {
    if (address % 2 != 0) {
        throw std::runtime_error("Unaligned 16-bit write at address: 0x" + std::to_string(address));
    }
    
    write8(address, value & 0xFF);
    write8(address + 1, (value >> 8) & 0xFF);
}

void Memory::write32(uint32_t address, uint32_t value) {
    if (address % 4 != 0) {
        throw std::runtime_error("Unaligned 32-bit write at address: 0x" + std::to_string(address));
    }
    
    write8(address, value & 0xFF);
    write8(address + 1, (value >> 8) & 0xFF);
    write8(address + 2, (value >> 16) & 0xFF);
    write8(address + 3, (value >> 24) & 0xFF);
}

void Memory::writeBytes(uint32_t address, const std::vector<uint8_t>& data) {
    if (!isValidAddress(address) || !isValidAddress(address + data.size() - 1)) {
        throw std::out_of_range("Invalid memory range for bulk write");
    }
    
    if (isRAMAddress(address)) {
        uint32_t offset = address - RAM_BASE;
        std::memcpy(&ram[offset], data.data(), data.size());
    }
}

std::vector<uint8_t> Memory::readBytes(uint32_t address, size_t length) const {
    if (!isValidAddress(address) || !isValidAddress(address + length - 1)) {
        throw std::out_of_range("Invalid memory range for bulk read");
    }
    
    std::vector<uint8_t> data(length);
    for (size_t i = 0; i < length; i++) {
        data[i] = read8(address + i);
    }
    return data;
}

bool Memory::isValidAddress(uint32_t address) const {
    return isRAMAddress(address) || isPeripheralAddress(address);
}

bool Memory::isRAMAddress(uint32_t address) const {
    return address >= RAM_BASE && address <= RAM_END;
}

bool Memory::isPeripheralAddress(uint32_t address) const {
    // Peripheral space is typically 0x3FF00000 - 0x3FF7FFFF
    return address >= 0x3FF00000 && address < RAM_BASE;
}

void Memory::mapPeripheral(uint32_t address, uint32_t peripheralId) {
    peripheralMappings[address] = peripheralId;
}

void Memory::unmapPeripheral(uint32_t address) {
    peripheralMappings.erase(address);
}

uint32_t Memory::getPeripheralId(uint32_t address) const {
    auto it = peripheralMappings.find(address);
    return (it != peripheralMappings.end()) ? it->second : 0;
}

void Memory::dumpMemory(uint32_t address, size_t length) const {
    std::cout << "Memory dump at 0x" << std::hex << address << ":" << std::endl;
    
    for (size_t i = 0; i < length; i += 16) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << (address + i) << ": ";
        
        for (size_t j = 0; j < 16 && (i + j) < length; j++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(read8(address + i + j)) << " ";
        }
        std::cout << std::dec << std::endl;
    }
} 