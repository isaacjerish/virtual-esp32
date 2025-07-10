#pragma once

#include <cstdint>
#include "Memory.h"

// Forward declaration
class Memory;


class XtensaLX6 {
private:
    Memory* memory;
    
    // CPU registers
    uint32_t registers[16];  // A0-A15
    uint32_t pc;             // Program Counter
    
    // Instruction execution helpers
    uint32_t fetchInstruction();
    void decodeAndExecute(uint32_t instruction);
    
    // Individual instruction implementations
    void executeNOP(uint32_t instruction);
    void executeL32IN(uint32_t instruction);
    void executeS32IN(uint32_t instruction);
    void executeADDN(uint32_t instruction);
    void executeJMP(uint32_t instruction);

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
    
    // Memory access (delegated to Memory class)
    uint32_t readMemory(uint32_t address) const;
    void writeMemory(uint32_t address, uint32_t value);
    
    // Debug/status
    void dumpRegisters() const;
    void dumpPC() const;
}; 