#include "Peripheral.h"

Peripheral::Peripheral(uint32_t baseAddr, uint32_t peripheralSize) 
    : baseAddress(baseAddr), size(peripheralSize) {
}

bool Peripheral::isInRange(uint32_t address) const {
    return address >= baseAddress && address < (baseAddress + size);
} 