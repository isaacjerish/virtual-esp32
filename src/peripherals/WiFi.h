#pragma once

#include "Peripheral.h"
#include <string>
#include <vector>
#include <queue>


class WiFi : public Peripheral {
private:
    uint32_t controlRegister;    
    uint32_t statusRegister;     
    uint32_t dataRegister;       
    uint32_t addressRegister;   
    uint32_t responseRegister;  
    
    bool connected;
    bool requestPending;
    std::string currentUrl;
    std::queue<uint8_t> responseBuffer;
    
    static constexpr uint32_t WIFI_CONTROL_OFFSET = 0x00;
    static constexpr uint32_t WIFI_STATUS_OFFSET = 0x04;
    static constexpr uint32_t WIFI_DATA_OFFSET = 0x08;
    static constexpr uint32_t WIFI_ADDRESS_OFFSET = 0x0C;
    static constexpr uint32_t WIFI_RESPONSE_OFFSET = 0x10;

public:
    WiFi();
    ~WiFi() = default;

    uint8_t read8(uint32_t offset) const override;
    uint16_t read16(uint32_t offset) const override;
    uint32_t read32(uint32_t offset) const override;
    
    void write8(uint32_t offset, uint8_t value) override;
    void write16(uint32_t offset, uint16_t value) override;
    void write32(uint32_t offset, uint32_t value) override;
    
    void reset() override;
    void update() override;
    void dumpRegisters() const override;
    
    bool connect();
    void disconnect();
    bool sendHttpRequest(const std::string& url);
    std::string getResponse();
    bool isConnected() const { return connected; }
}; 