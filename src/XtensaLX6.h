#pragma once

#include <cstdint>
#include <functional>
#include "Memory.h"

class Memory;


class XtensaLX6 {
private:
    Memory* memory;
    
    std::function<uint32_t(uint32_t)> peripheralReadCallback;
    std::function<void(uint32_t, uint32_t)> peripheralWriteCallback;
    
    uint32_t registers[16]; 
    uint32_t pc;             
    
    uint32_t fetchInstruction();
    void decodeAndExecute(uint32_t instruction);
    
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

    void execute();
    void reset();

    uint32_t getRegister(uint8_t reg) const;
    void setRegister(uint8_t reg, uint32_t value);
    
    uint32_t getPC() const { return pc; }
    void setPC(uint32_t newPC) { pc = newPC; }
    
    uint32_t readMemory(uint32_t address) const;
    void writeMemory(uint32_t address, uint32_t value);
    
    void setPeripheralReadCallback(std::function<uint32_t(uint32_t)> callback);
    void setPeripheralWriteCallback(std::function<void(uint32_t, uint32_t)> callback);
    
    void dumpRegisters() const;
    void dumpPC() const;
}; 