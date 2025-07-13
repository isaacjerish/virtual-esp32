#include "Emulator.h"
#include <iostream>
#include <stdexcept>

Emulator::Emulator() : running(false), cycles(0) {
    // Initialize system components
    memory = std::make_unique<Memory>();
    cpu = std::make_unique<XtensaLX6>(memory.get());
    
    // Set up peripheral callbacks
    memory->setPeripheralCallbacks(
        [this](uint32_t addr) { return readPeripheral8(addr); },
        [this](uint32_t addr) { return readPeripheral16(addr); },
        [this](uint32_t addr) { return readPeripheral32(addr); },
        [this](uint32_t addr, uint8_t val) { writePeripheral8(addr, val); },
        [this](uint32_t addr, uint16_t val) { writePeripheral16(addr, val); },
        [this](uint32_t addr, uint32_t val) { writePeripheral32(addr, val); }
    );
    
    cpu->setPeripheralReadCallback([this](uint32_t addr) { return readPeripheral32(addr); });
    cpu->setPeripheralWriteCallback([this](uint32_t addr, uint32_t val) { writePeripheral32(addr, val); });
    
    // Add UART peripheral
    auto uart = std::make_unique<UART>();
    addPeripheral(std::move(uart));
    
    std::cout << "ESP32 Emulator initialized" << std::endl;
}

void Emulator::loadFirmware(const std::vector<uint8_t>& firmware) {
    if (firmware.empty()) {
        throw std::runtime_error("Firmware is empty");
    }
    
    // Load firmware into memory starting at 0x40080000 (typical ESP32 entry point)
    const uint32_t firmwareBase = 0x40080000;
    memory->writeBytes(firmwareBase, firmware);
    
    // Set CPU PC to firmware entry point
    cpu->setPC(firmwareBase);
    
    std::cout << "Firmware loaded at 0x" << std::hex << firmwareBase << std::dec << std::endl;
}

void Emulator::run() {
    running = true;
    std::cout << "Starting emulation..." << std::endl;
    
    while (running) {
        step();
    }
}

void Emulator::step() {
    if (!running) return;
    
    try {
        // Execute one CPU cycle
        cpu->execute();
        cycles++;
        
        // Handle any peripheral updates
        for (auto& peripheral : peripherals) {
            peripheral->update();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Emulation error at cycle " << cycles << ": " << e.what() << std::endl;
        stop();
    }
}

void Emulator::stop() {
    running = false;
    std::cout << "Emulation stopped after " << cycles << " cycles" << std::endl;
}

void Emulator::addPeripheral(std::unique_ptr<Peripheral> peripheral) {
    peripherals.push_back(std::move(peripheral));
}

Peripheral* Emulator::getPeripheral(uint32_t address) const {
    for (const auto& peripheral : peripherals) {
        if (peripheral->isInRange(address)) {
            return peripheral.get();
        }
    }
    return nullptr;
} 
// TODO: Implement peripheral interrupt handling

uint8_t Emulator::readPeripheral8(uint32_t address) const {
    Peripheral* peripheral = getPeripheral(address);
    if (peripheral) {
        uint32_t offset = address - peripheral->getBaseAddress();
        return peripheral->read8(offset);
    }
    return 0;
}

uint16_t Emulator::readPeripheral16(uint32_t address) const {
    Peripheral* peripheral = getPeripheral(address);
    if (peripheral) {
        uint32_t offset = address - peripheral->getBaseAddress();
        return peripheral->read16(offset);
    }
    return 0;
}

uint32_t Emulator::readPeripheral32(uint32_t address) const {
    Peripheral* peripheral = getPeripheral(address);
    if (peripheral) {
        uint32_t offset = address - peripheral->getBaseAddress();
        return peripheral->read32(offset);
    }
    return 0;
}

void Emulator::writePeripheral8(uint32_t address, uint8_t value) {
    Peripheral* peripheral = getPeripheral(address);
    if (peripheral) {
        uint32_t offset = address - peripheral->getBaseAddress();
        peripheral->write8(offset, value);
    }
}

void Emulator::writePeripheral16(uint32_t address, uint16_t value) {
    Peripheral* peripheral = getPeripheral(address);
    if (peripheral) {
        uint32_t offset = address - peripheral->getBaseAddress();
        peripheral->write16(offset, value);
    }
}

void Emulator::writePeripheral32(uint32_t address, uint32_t value) {
    Peripheral* peripheral = getPeripheral(address);
    if (peripheral) {
        uint32_t offset = address - peripheral->getBaseAddress();
        peripheral->write32(offset, value);
    }
}