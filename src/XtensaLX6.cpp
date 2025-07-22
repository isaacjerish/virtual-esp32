#include "XtensaLX6.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

XtensaLX6::XtensaLX6(Memory* mem) : memory(mem), pc(0) {
    for (int i = 0; i < 16; i++) {
        registers[i] = 0;
    }
    
    std::cout << "Xtensa LX6 CPU initialized" << std::endl;
}

void XtensaLX6::execute() {
    uint32_t instruction = fetchInstruction();
    
    decodeAndExecute(instruction);
    
}

void XtensaLX6::reset() {
    pc = 0;
    for (int i = 0; i < 16; i++) {
        registers[i] = 0;
    }
}

uint32_t XtensaLX6::fetchInstruction() {
    return memory->read32(pc);
}

void XtensaLX6::decodeAndExecute(uint32_t instruction) {
    uint8_t opcode = instruction & 0x0F;
    
    switch (opcode) {
        case 0x0: // L32I.N
            executeL32IN(instruction);
            break;
        case 0x1: // S32I.N
            executeS32IN(instruction);
            break;
        case 0x2: // ADD.N
            executeADDN(instruction);
            break;
        case 0x3: // JMP
            executeJMP(instruction);
            break;
        case 0x4: // NOP
            executeNOP(instruction);
            break;
        case 0x5: // SUB.N
            executeSUBN(instruction);
            break;
        case 0x6: // MOV.N
            executeMOVN(instruction);
            break;
        case 0x7: // BEQ.N
            executeBEQN(instruction);
            break;
        default:
            throw std::runtime_error("Unknown instruction opcode: " + std::to_string(opcode));
    }
}

void XtensaLX6::executeL32IN(uint32_t instruction) {

    uint8_t ar = (instruction >> 8) & 0x0F; 
    uint8_t as = (instruction >> 12) & 0x0F; 
    uint8_t imm8 = (instruction >> 4) & 0xFF;
    
    uint32_t address = registers[as] + imm8;
    uint32_t value = readMemory(address);
    registers[ar] = value;
    
    pc += 2; 
}

void XtensaLX6::executeS32IN(uint32_t instruction) {

    uint8_t ar = (instruction >> 8) & 0x0F;  
    uint8_t as = (instruction >> 12) & 0x0F; 
    uint8_t imm8 = (instruction >> 4) & 0xFF; 
    
    uint32_t address = registers[as] + imm8;
    writeMemory(address, registers[ar]);
    
    pc += 2;
}

void XtensaLX6::executeADDN(uint32_t instruction) {

    uint8_t ar = (instruction >> 8) & 0x0F; 
    uint8_t as = (instruction >> 12) & 0x0F; 
    uint8_t at = (instruction >> 4) & 0x0F; 
    
    registers[ar] = registers[as] + registers[at];
    
    pc += 2; // Narrow instruction is 2 bytes
}

void XtensaLX6::executeJMP(uint32_t instruction) {

    uint8_t ar = (instruction >> 8) & 0x0F;
    
    pc = registers[ar];
}

void XtensaLX6::executeNOP(uint32_t instruction) {
   
    pc += 2; 
}

void XtensaLX6::executeSUBN(uint32_t instruction) {
    uint8_t ar = (instruction >> 8) & 0x0F;  
    uint8_t as = (instruction >> 12) & 0x0F; 
    uint8_t at = (instruction >> 4) & 0x0F;  
    
    registers[ar] = registers[as] - registers[at];
    
    pc += 2; 
}

void XtensaLX6::executeMOVN(uint32_t instruction) {
    uint8_t ar = (instruction >> 8) & 0x0F; 
    uint8_t as = (instruction >> 12) & 0x0F; 
    
    registers[ar] = registers[as];
    
    pc += 2;
}

void XtensaLX6::executeBEQN(uint32_t instruction) {
    uint8_t as = (instruction >> 8) & 0x0F;   
    uint8_t at = (instruction >> 12) & 0x0F; 
    int8_t imm8 = (instruction >> 4) & 0xFF;
    
    if (registers[as] == registers[at]) {
        pc += (imm8 * 2); 
    }
    
    pc += 2; 
}

uint32_t XtensaLX6::getRegister(uint8_t reg) const {
    if (reg >= 16) {
        throw std::out_of_range("Register index out of range: " + std::to_string(reg));
    }
    return registers[reg];
}

void XtensaLX6::setRegister(uint8_t reg, uint32_t value) {
    if (reg >= 16) {
        throw std::out_of_range("Register index out of range: " + std::to_string(reg));
    }
    registers[reg] = value;
}

uint32_t XtensaLX6::readMemory(uint32_t address) const {
    if (address >= 0x3FF00000 && address < 0x3FF80000) {
        if (peripheralReadCallback) {
            return peripheralReadCallback(address);
        }
        return 0;
    }
    
    return memory->read32(address);
}

void XtensaLX6::writeMemory(uint32_t address, uint32_t value) {
    if (address >= 0x3FF00000 && address < 0x3FF80000) {
        if (peripheralWriteCallback) {
            peripheralWriteCallback(address, value);
        }
        return;
    }
    
    memory->write32(address, value);
}

void XtensaLX6::dumpRegisters() const {
    std::cout << "CPU Registers:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "  A" << i << ": 0x" << std::hex << std::setw(8) << std::setfill('0') 
                  << registers[i] << std::dec << std::endl;
    }
}

void XtensaLX6::dumpPC() const {
    std::cout << "PC: 0x" << std::hex << pc << std::dec << std::endl;
} 