#pragma once

#include <cstdint>
#include <functional>
#include "Memory.h"

// Forward declaration
class Memory;

/**
 * Xtensa LX6 CPU Emulator
 */
class XtensaLX6 {
private:
    Memory* memory;
    
    // Peripheral access callback
    std::function<uint32_t(uint32_t)> peripheralReadCallback;
    std::function<void(uint32_t, uint32_t)> peripheralWriteCallback;
    
    // CPU registers
    uint32_t registers[16];  // A0-A15
    uint32_t pc;             // Program Counter
    
    // Instruction execution helpers
    uint32_t fetchInstruction();
    void decodeAndExecute(uint32_t instruction);
    
    // Individual instruction implementations
    void executeL32IN(uint32_t instruction);
    void executeS32IN(uint32_t instruction);
    void executeADDN(uint32_t instruction);
    void executeJMP(uint32_t instruction);
    void executeNOP(uint32_t instruction);
    void executeSUBN(uint32_t instruction);
    void executeMOVN(uint32_t instruction);
    void executeBEQN(uint32_t instruction);

public:
    explicit XtensaLX6(Memory* mem);
    ~XtensaLX6() = default;

    // Core execution
    void execute();
    void reset();

    // Register access
    uint32_t getRegister(uint8_t reg) const;
    void setRegister(uint8_t reg, uint32_t value);
    
    // PC access
    uint32_t getPC() const { return pc; }
    void setPC(uint32_t newPC) { pc = newPC; }
    
    // Memory access (peripheral-aware)
    uint32_t readMemory(uint32_t address) const;
    void writeMemory(uint32_t address, uint32_t value);
    
    // Peripheral callback registration
    void setPeripheralReadCallback(std::function<uint32_t(uint32_t)> callback);
    void setPeripheralWriteCallback(std::function<void(uint32_t, uint32_t)> callback);
    
    // Debug/status
    void dumpRegisters() const;
    void dumpPC() const;
}; 