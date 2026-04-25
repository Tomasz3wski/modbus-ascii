#pragma once

#include "../serial/SerialPort.hpp"
#include "Frame.hpp"

#include <string>
#include <atomic>
#include <functional>

class Slave {
public:
    Slave(SerialPort& port);

    void setOnTextReceived(std::function<void(const std::string&)> callback);

    ParsedFrame receiveRequest();
    void processRequest(const ParsedFrame& frame);

    void setAddress(uint8_t address);      // 1 - 247
    void setInterCharTimeout(int ms);      // 0 - 1000 ms, step 10ms

    uint8_t getAddress() const;
    int getInterCharTimeout() const;

    void run();
    void stop();

private:
    SerialPort& mPort;
    uint8_t mAddress;
    int interCharTimeout;
    std::string storedText;
    std::atomic<bool> isRunning;
    std::function<void(const std::string&)> onTextReceived;

    enum class State { IDLE, RECEIVING };
};