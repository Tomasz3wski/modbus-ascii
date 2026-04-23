#pragma once

#include "../serial/SerialPort.hpp"
#include "Frame.hpp"

class Slave {
public:
    Slave(SerialPort& port);

    ParsedFrame receiveRequest();

    void setAddress(uint8_t address);      // 1 - 247
    void setInterCharTimeout(int ms);      // 0 - 1000 ms, step 10ms

    uint8_t getAddress() const;
    int getInterCharTimeout() const;

private:
    SerialPort& mPort;
    uint8_t mAddress;
    int interCharTimeout;

    enum class State { IDLE, RECEIVING };
};