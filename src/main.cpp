#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Emulator.h"

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <firmware.bin>" << std::endl;
    std::cout << "  Loads and executes ESP32 firmware binary" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string firmwarePath = argv[1];
    
    // Load firmware binary
    std::ifstream firmwareFile(firmwarePath, std::ios::binary);
    if (!firmwareFile.is_open()) {
        std::cerr << "Error: Could not open firmware file: " << firmwarePath << std::endl;
        return 1;
    }

    // Read firmware into vector
    std::vector<uint8_t> firmware;
    firmwareFile.seekg(0, std::ios::end);
    firmware.resize(firmwareFile.tellg());
    firmwareFile.seekg(0, std::ios::beg);
    firmwareFile.read(reinterpret_cast<char*>(firmware.data()), firmware.size());
    firmwareFile.close();

    std::cout << "Loaded firmware: " << firmwarePath << " (" << firmware.size() << " bytes)" << std::endl;

    // Create and run emulator
    try {
        Emulator emulator;
        emulator.loadFirmware(firmware);
        emulator.run();
    } catch (const std::exception& e) {
        std::cerr << "Emulator error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 
// TODO: Support memory-mapped peripheral initialization via config file