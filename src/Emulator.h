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

    // Core emulation methods
    void loadFirmware(const std::vector<uint8_t>& firmware);
    void run();
    void step();
    void stop();

    // System access
    XtensaLX6* getCPU() const { return cpu.get(); }
    Memory* getMemory() const { return memory.get(); }
    
    // Peripheral management
    void addPeripheral(std::unique_ptr<Peripheral> peripheral);
    Peripheral* getPeripheral(uint32_t address) const;

    // Statistics
    uint64_t getCycles() const { return cycles; }
    bool isRunning() const { return running; }
}; 