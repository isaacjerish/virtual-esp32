#include "Emulator.h"
#include <iostream>
#include <stdexcept>

Emulator::Emulator() : running(false), cycles(0) {
    // Initialize system components
    memory = std::make_unique<Memory>();
    cpu = std::make_unique<XtensaLX6>(memory.get());
    
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