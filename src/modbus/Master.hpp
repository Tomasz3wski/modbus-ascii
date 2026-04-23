#pragma once

#include "../serial/SerialPort.hpp"
#include "Frame.hpp"

#include <vector>

class Master {
public:
    Master(SerialPort& port);

    bool sendFrame(uint8_t address, uint8_t function, const std::vector<uint8_t>& data);
    ParsedFrame receiveResponse();
    ParsedFrame sendTransaction(uint8_t address, uint8_t function, const std::vector<uint8_t>& data);

    void setTransactionTimeout(int ms); // 0 - 10000 ms, step 100ms
    void setRetryCount(int count); // 0 - 5
    void setInterCharTimeout(int ms); // 0 - 1000 ms, step 10ms

    int getTransactionTimeout() const;
    int getRetryCount() const;
    int getInterCharTimeout() const;

private:
    int transactionTimeout;
    int retryCount;
    int interCharTimeout;
    SerialPort& mPort;
};