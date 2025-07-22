#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include "XtensaLX6.h"
#include "Memory.h"
#include "peripherals/Peripheral.h"


class Emulator {
private:
    std::unique_ptr<XtensaLX6> cpu;
    std::unique_ptr<Memory> memory;
    std::vector<std::unique_ptr<Peripheral>> peripherals;
    
    bool running;
    uint64_t cycles;

public:
    Emulator();
    ~Emulator() = default;

    void loadFirmware(const std::vector<uint8_t>& firmware);
    void run();
    void step();
    void stop();

    XtensaLX6* getCPU() const { return cpu.get(); }
    Memory* getMemory() const { return memory.get(); }
    
    void addPeripheral(std::unique_ptr<Peripheral> peripheral);
    Peripheral* getPeripheral(uint32_t address) const;

    uint64_t getCycles() const { return cycles; }
    bool isRunning() const { return running; }

    uint8_t readPeripheral8(uint32_t address) const;
    uint16_t readPeripheral16(uint32_t address) const;
    uint32_t readPeripheral32(uint32_t address) const;
    
    void writePeripheral8(uint32_t address, uint8_t value);
    void writePeripheral16(uint32_t address, uint16_t value);
    void writePeripheral32(uint32_t address, uint32_t value);
}; 