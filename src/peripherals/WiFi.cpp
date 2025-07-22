#include "WiFi.h"
#include <iostream>
#include <iomanip>

WiFi::WiFi() : Peripheral(0x3FF50000, 0x100),
               controlRegister(0), statusRegister(0),
               dataRegister(0), addressRegister(0),
               responseRegister(0), connected(false),
               requestPending(false) {
    std::cout << "WiFi peripheral initialized at 0x" << std::hex << baseAddress << std::dec << std::endl;
}

uint8_t WiFi::read8(uint32_t offset) const {
    switch (offset) {
        case WIFI_CONTROL_OFFSET:
            return controlRegister & 0xFF;
        case WIFI_STATUS_OFFSET:
            return statusRegister & 0xFF;
        case WIFI_DATA_OFFSET:
            return dataRegister & 0xFF;
        case WIFI_ADDRESS_OFFSET:
            return addressRegister & 0xFF;
        case WIFI_RESPONSE_OFFSET:
            return responseRegister & 0xFF;
        default:
            return 0;
    }
}

uint16_t WiFi::read16(uint32_t offset) const {
    if (offset % 2 != 0) return 0;
    
    uint16_t value = 0;
    value |= static_cast<uint16_t>(read8(offset));
    value |= static_cast<uint16_t>(read8(offset + 1)) << 8;
    return value;
}

uint32_t WiFi::read32(uint32_t offset) const {
    if (offset % 4 != 0) return 0;
    
    uint32_t value = 0;
    value |= static_cast<uint32_t>(read8(offset));
    value |= static_cast<uint32_t>(read8(offset + 1)) << 8;
    value |= static_cast<uint32_t>(read8(offset + 2)) << 16;
    value |= static_cast<uint32_t>(read8(offset + 3)) << 24;
    return value;
}

void WiFi::write8(uint32_t offset, uint8_t value) {
    switch (offset) {
        case WIFI_CONTROL_OFFSET:
            controlRegister = value;
            if (value & 0x01) {  
                connect();
            }
            if (value & 0x02) {  
                disconnect();
            }
            if (value & 0x04) { 
                std::cout << "WiFi: HTTP request triggered" << std::endl;
            }
            break;
        case WIFI_STATUS_OFFSET:
            statusRegister = value;
            break;
        case WIFI_DATA_OFFSET:
            dataRegister = value;
            break;
        case WIFI_ADDRESS_OFFSET:
            addressRegister = (addressRegister & 0xFFFFFF00) | value;
            break;
        case WIFI_RESPONSE_OFFSET:
            responseRegister = value;
            break;
    }
}

void WiFi::write16(uint32_t offset, uint16_t value) {
    if (offset % 2 != 0) return;
    
    write8(offset, value & 0xFF);
    write8(offset + 1, (value >> 8) & 0xFF);
}

void WiFi::write32(uint32_t offset, uint32_t value) {
    if (offset % 4 != 0) return;
    
    write8(offset, value & 0xFF);
    write8(offset + 1, (value >> 8) & 0xFF);
    write8(offset + 2, (value >> 16) & 0xFF);
    write8(offset + 3, (value >> 24) & 0xFF);
}

void WiFi::reset() {
    controlRegister = 0;
    statusRegister = 0;
    dataRegister = 0;
    addressRegister = 0;
    responseRegister = 0;
    connected = false;
    requestPending = false;
    currentUrl.clear();
    
    while (!responseBuffer.empty()) responseBuffer.pop();
}

void WiFi::update() {
    if (connected) {
        statusRegister |= 0x01; 
    } else {
        statusRegister &= ~0x01;
    }
    
    if (requestPending) {
        statusRegister |= 0x02; 
    } else {
        statusRegister &= ~0x02;
    }
    
    if (!responseBuffer.empty()) {
        statusRegister |= 0x04;  
    } else {
        statusRegister &= ~0x04;
    }
}

void WiFi::dumpRegisters() const {
    std::cout << "WiFi Registers:" << std::endl;
    std::cout << "  Control:   0x" << std::hex << std::setw(8) << std::setfill('0') << controlRegister << std::dec << std::endl;
    std::cout << "  Status:    0x" << std::hex << std::setw(8) << std::setfill('0') << statusRegister << std::dec << std::endl;
    std::cout << "  Data:      0x" << std::hex << std::setw(8) << std::setfill('0') << dataRegister << std::dec << std::endl;
    std::cout << "  Address:   0x" << std::hex << std::setw(8) << std::setfill('0') << addressRegister << std::dec << std::endl;
    std::cout << "  Response:  0x" << std::hex << std::setw(8) << std::setfill('0') << responseRegister << std::dec << std::endl;
    std::cout << "  Connected: " << (connected ? "Yes" : "No") << std::endl;
}

bool WiFi::connect() {
    connected = true;
    std::cout << "WiFi: Connected to network" << std::endl;
    return true;
}

void WiFi::disconnect() {
    connected = false;
    std::cout << "WiFi: Disconnected from network" << std::endl;
}

bool WiFi::sendHttpRequest(const std::string& url) {
    if (!connected) {
        std::cout << "WiFi: Not connected, cannot send request" << std::endl;
        return false;
    }
    
    currentUrl = url;
    requestPending = true;
    std::cout << "WiFi: Sending HTTP request to " << url << std::endl;
    
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    for (char c : response) {
        responseBuffer.push(static_cast<uint8_t>(c));
    }
    
    requestPending = false;
    return true;
}

std::string WiFi::getResponse() {
    std::string response;
    while (!responseBuffer.empty()) {
        response += static_cast<char>(responseBuffer.front());
        responseBuffer.pop();
    }
    return response;
} 